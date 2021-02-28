using System;
using System.IO;
using System.Linq;

namespace HashCode {
    class Program {
        static void Main(string[] args) {
            Console.WriteLine("Reading initial data");

            Model model;
            using (var sr = new StreamReader(args[0])) {
                var n = sr
                    .ReadLine()
                    .Split(' ')
                    .Select(s => int.Parse(s))
                    .ToArray();

                model = new Model(
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
            }

            Console.WriteLine("Data read succesfully. Configuring traffic lights.");

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

            Console.WriteLine("Running simulation.");
            Console.WriteLine();
            for (int timer = 0; timer <= model.Duration; timer++) {
                Console.WriteLine("Timer is {0} >>>>>>>>>>>", timer);

                // deal with traffic lights
                foreach (var intersection in model.Intersections) {
                    Console.WriteLine("Checking intersection {0}", intersection.Id);
                    if (intersection.TrafficLights.Count == 0) {
                        Console.WriteLine("Intersection {0} has no traffic lights - it's always RED", intersection.Id);
                        continue;
                    }
                    intersection.SwitchTrafficLight();
                    foreach (var tl in intersection.TrafficLights) {
                        if (tl.Light == TrafficLight.State.Green) {
                            Console.WriteLine("Traffic light on street {0} is {1}. Seconds left {2}", tl.Street.Name, tl.Light, tl.GreenSecondsLeft);
                        } else {
                            Console.WriteLine("Traffic light on street {0} is {1}", tl.Street.Name, tl.Light);
                        }
                    }
                }

                // deal with cars
                foreach (var car in model.Cars) {
                    Console.WriteLine("Checking car {0}", car.Id);
                    if (car.Finished) {
                        Console.WriteLine("Car {0} has already finished - skipping", car.Id);
                        continue;
                    }
                    car.Move();
                    if (car.Finished) {
                        var score = model.Bonus + (model.Duration - timer);
                        Console.WriteLine(
                            "Car {0} has just finished with score {1} = {2} + ({3} - {4})",
                            car.Id, score, model.Bonus, model.Duration, timer
                        );
                        model.Score += score;
                    } else {
                        Console.WriteLine(
                            "Car {0} is currently on street {1}. Left on current street {2}",
                            car.Id, car.CurrentStreet.Name, car.LeftOnCurrentStreet
                        );
                    }
                }
                Console.WriteLine("Timer {0} finished <<<<<<<<<<<", timer);
                Console.WriteLine();
            }

            Console.WriteLine("Simulation completed. Total score is {0}", model.Score);
        }
    }
}
