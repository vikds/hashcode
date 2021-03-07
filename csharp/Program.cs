using System;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Linq;

namespace HashCode {
    static class Program {
        static void Main(string[] args) {
            var parameters = new Parameters(args);

            Logger.Init(parameters.Verbose);
            Logger.Info($"Reading initial data from {parameters.InputFile}");
            var model = ModelLoader.ImportModel(parameters.InputFile);
            Console.Write("Model is ready.");
            Console.Write($"\n\tStreets: {model.Streets.Length}");
            Console.Write($"\n\tIntersections: {model.Intersections.Length}");
            Console.Write($"\n\tCars: {model.Cars.Length}");
            Console.WriteLine($"\n\tDuration:{model.Duration}");

            var bestModel = new Model() {
                Score = int.MinValue,
            };
            var simulator = new Simulator(model);

            var totalWatch = Stopwatch.StartNew();
            if (parameters.Parallel) {
                var lockObject = new object();
                var parallelOptions = new ParallelOptions();
                if (parameters.ParallelSimulations > 0) parallelOptions.MaxDegreeOfParallelism = parameters.ParallelSimulations;
                Parallel.For(0, 10, parallelOptions, i => {
                    Logger.Info($"Simulation {i + 1} started");
                    var freshModel = simulator.RunSimulation(out long elapsedMs);
                    Logger.Info($"Simulation {i + 1} completed. Total score is {freshModel.Score}. Took {elapsedMs / 1000.0:#.##} seconds");

                    lock (lockObject) {
                        if (bestModel.Score < freshModel.Score) {
                            bestModel = freshModel;
                        }
                    }
                    simulator.UpdateSchedule();
                });
            } else {
                for (var i = 0; i < 20; i++) {
                    Logger.Info($"Simulation {i + 1} started");
                    var freshModel = simulator.RunSimulation(out long elapsedMs);
                    Logger.Info($"Simulation {i + 1} completed. Total score is {freshModel.Score}. Took {elapsedMs / 1000.0:#.##} seconds");

                    if (bestModel.Score < freshModel.Score) {
                        bestModel = freshModel;
                    }
                    simulator.UpdateSchedule();
                }
            }
            totalWatch.Stop();
            Logger.Info($"Total time is {totalWatch.ElapsedMilliseconds / 1000.0:#.##} and the best score is {bestModel.Score}");
            ModelLoader.ExportModel(bestModel, parameters.InputFile, parameters.OutputFolder);
        }
    }
}
