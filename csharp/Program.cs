using System;
using System.IO;
using System.Linq;

namespace HashCode {
    class Program {
        static void Main(string[] args) {
            var parameters = new Parameters(args);

            Logger.Init(parameters.Verbose);
            Logger.Log($"Reading initial data from {parameters.InputFile}");

            var model = ImportModel(parameters.InputFile);

            Logger.Log("Data read successfully. Configuring traffic lights.");

            model.Intersections[1].TrafficLights.Add(new TrafficLight() {
                Street = model.Intersections[1].Incoming.Single(s => s.Name == "rue-d-athenes"),
                GreenDuration = 2,
            });
            model.Intersections[1].TrafficLights.Add(new TrafficLight() {
                Street = model.Intersections[1].Incoming.Single(s => s.Name == "rue-d-amsterdam"),
                GreenDuration = 1,
            });

            model.Intersections[0].TrafficLights.Add(new TrafficLight() {
                Street = model.Intersections[0].Incoming.Single(s => s.Name == "rue-de-londres"),
                GreenDuration = 2,
            });

            model.Intersections[2].TrafficLights.Add(new TrafficLight() {
                Street = model.Intersections[2].Incoming.Single(s => s.Name == "rue-de-moscou"),
                GreenDuration = 1,
            });

            foreach (var i in model.Intersections) {
                foreach (var t in i.TrafficLights) {
                    t.Street.TrafficLight = t;
                }
                i.SetInitialState();
            }

            foreach (var c in model.Cars) {
                c.SetInitialState();
            }

            Logger.Log("Running simulation.");
            Logger.Divider();
            for (int timer = 0; timer <= model.Duration; timer++) {
                Logger.Log($"Timer is {timer} >>>>>>>>>>>");

                // deal with traffic lights
                foreach (var intersection in model.Intersections) {
                    intersection.CarJustPassed = false;
                    Logger.Log($"Checking intersection {intersection.Id}");
                    if (intersection.TrafficLights.Count == 0) {
                        Logger.Log($"Intersection {intersection.Id} has no traffic lights - it's always Red");
                        continue;
                    }
                    intersection.SwitchTrafficLight();
                    foreach (var tl in intersection.TrafficLights) {
                        if (tl.State == TrafficLight.Colors.Green) {
                            Logger.Log($"Traffic light on street {tl.Street.Name} is {tl.State}. Seconds left {tl.GreenSecondsLeft}");
                        } else {
                            Logger.Log($"Traffic light on street {tl.Street.Name} is {tl.State}");
                        }
                    }
                }

                // deal with cars
                foreach (var car in model.Cars) {
                    Logger.Log($"Checking car {car.Id}");
                    if (car.Finished) {
                        Logger.Log($"Car {car.Id} has already finished - skipping");
                        continue;
                    }
                    car.Move();
                    if (car.Finished) {
                        var score = model.Bonus + (model.Duration - timer);
                        Logger.Log(
                            $"Car {car.Id} has just finished with score {score} = {model.Bonus} + ({model.Duration} - {timer})"
                        );
                        model.Score += score;
                    } else {
                        Logger.Log($"Car {car.Id} is currently on street {car.CurrentStreet.Name}. Left on current street {car.LeftOnCurrentStreet}");
                    }
                }
                Logger.Log($"Timer {timer} finished <<<<<<<<<<<");
                Logger.Divider();
            }

            Console.WriteLine($"Simulation completed. Total score is {model.Score}");
            ExportModel(model, parameters.InputFile, parameters.OutputFolder);
        }

        private static Model ImportModel(string fileName) {
            using (var sr = new StreamReader(fileName)) {
                var n = sr
                    .ReadLine()
                    .Split(' ')
                    .Select(s => int.Parse(s))
                    .ToArray();

                var model = new Model(
                    duration: n[0],
                    intersectionsNum: n[1],
                    streetsNum: n[2],
                    carsNum: n[3],
                    bonus: n[4]
                );

                for (var i = 0; i < model.Streets.Length; i++) {
                    var l = sr
                        .ReadLine()
                        .Split(' ');
                    model.Streets[i] = new Street() {
                        StartsAt = model.Intersections[int.Parse(l[0])],
                        EndsAt = model.Intersections[int.Parse(l[1])],
                        Name = l[2],
                        Length = int.Parse(l[3]),
                    };
                    model.Streets[i].StartsAt.Outcoming.Add(model.Streets[i]);
                    model.Streets[i].EndsAt.Incoming.Add(model.Streets[i]);
                }

                for (var i = 0; i < model.Cars.Length; i++) {
                    var l = sr
                        .ReadLine()
                        .Split(' ');
                    model.Cars[i] = new Car() {
                        Id = i,
                        Route = l
                            .Skip(1)
                            .Select(s => model.Streets.Single(ss => ss.Name == s))
                            .ToArray(),
                    };
                }
                return model;
            }
        }
        private static void ExportModel(Model model, string inputFileName, string outputFolderName) {
            var inputFileNameExtension = Path.GetExtension(inputFileName);
            var inputFileNameWithoutExtension = Path.GetFileNameWithoutExtension(inputFileName);

            var outputFileName = Path.Combine(
                outputFolderName,
                string.Format("{0}_{1}_Score_{2}",
                    inputFileNameWithoutExtension,
                    DateTime.Now.ToString("yyyy_MM_dd_HH_mm"),
                    model.Score
                )
            );
            outputFileName = Path.ChangeExtension(outputFileName, inputFileNameExtension);

            if (!Directory.Exists(outputFolderName)) {
                Directory.CreateDirectory(outputFolderName);
            }

            using (var sw = new StreamWriter(outputFileName)) {
                sw.Write(model.ExportSchedule());
                sw.Flush();
                sw.Close();
            }
        }
    }
}
