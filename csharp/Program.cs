using System;
using System.IO;
using System.Linq;

namespace HashCode {
    class Program {
        static void Main(string[] args) {
            Logger.Init(true);
            Logger.Log("Reading initial data from {0}", args[0]);

            var model = ImportModel(args[0]);

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
                Logger.Log("Timer is {0} >>>>>>>>>>>", timer);

                // deal with traffic lights
                foreach (var intersection in model.Intersections) {
                    intersection.CarJustPassed = false;
                    Logger.Log("Checking intersection {0}", intersection.Id);
                    if (intersection.TrafficLights.Count == 0) {
                        Logger.Log("Intersection {0} has no traffic lights - it's always Red", intersection.Id);
                        continue;
                    }
                    intersection.SwitchTrafficLight();
                    foreach (var tl in intersection.TrafficLights) {
                        if (tl.State == TrafficLight.Colors.Green) {
                            Logger.Log("Traffic light on street {0} is {1}. Seconds left {2}", tl.Street.Name, tl.State, tl.GreenSecondsLeft);
                        } else {
                            Logger.Log("Traffic light on street {0} is {1}", tl.Street.Name, tl.State);
                        }
                    }
                }

                // deal with cars
                foreach (var car in model.Cars) {
                    Logger.Log("Checking car {0}", car.Id);
                    if (car.Finished) {
                        Logger.Log("Car {0} has already finished - skipping", car.Id);
                        continue;
                    }
                    car.Move();
                    if (car.Finished) {
                        var score = model.Bonus + (model.Duration - timer);
                        Logger.Log(
                            "Car {0} has just finished with score {1} = {2} + ({3} - {4})",
                            car.Id, score, model.Bonus, model.Duration, timer
                        );
                        model.Score += score;
                    } else {
                        Logger.Log(
                            "Car {0} is currently on street {1}. Left on current street {2}",
                            car.Id, car.CurrentStreet.Name, car.LeftOnCurrentStreet
                        );
                    }
                }
                Logger.Log("Timer {0} finished <<<<<<<<<<<", timer);
                Logger.Divider();
            }

            Console.WriteLine("Simulation completed. Total score is {0}", model.Score);
            ExportModel(model, args[0], args[1]);
            // model.ExportSchedule(inputFileName: args[0], outputFolderName: args[1]);
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
