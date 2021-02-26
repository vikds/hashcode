﻿using System;
using System.IO;
using System.Linq;

namespace HashCode {
    class Program {
        static void Main(string[] args) {
            Console.WriteLine("Reading initial data");

            var model = new Model();
            using (var sr = new StreamReader(args[0])) {
                var n = sr
                    .ReadLine()
                    .Split(' ')
                    .Select(s => int.Parse(s))
                    .ToArray();

                model.Init(n[0], n[1], n[2], n[3], n[4]);

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

                for (var i = 0; i < model.Vehicles.Length; i++) {
                    var l = sr
                        .ReadLine()
                        .Split(' ');
                    model.Vehicles[i] = new Vehicle() {
                        Id = i,
                        Route = l
                            .Skip(1)
                            .Select(s => model.Streets.Single(ss => ss.Name == s))
                            .ToArray(),
                    };
                }
            }

            Console.WriteLine("Data read succesfully. Configuring traffic lights.");

            model.Intersections[1].TrafficLights.Add(new TrafficLight {
                Street = model.Intersections[1].Incoming.Single(s => s.Name == "rue-d-athenes"),
                GreenDuration = 2,
            });
            model.Intersections[1].TrafficLights.Add(new TrafficLight {
                Street = model.Intersections[1].Incoming.Single(s => s.Name == "rue-d-amsterdam"),
                GreenDuration = 1,
            });

            model.Intersections[0].TrafficLights.Add(new TrafficLight {
                Street = model.Intersections[0].Incoming.Single(s => s.Name == "rue-de-londres"),
                GreenDuration = 2,
            });

            model.Intersections[2].TrafficLights.Add(new TrafficLight {
                Street = model.Intersections[2].Incoming.Single(s => s.Name == "rue-de-moscou"),
                GreenDuration = 1,
            });

            foreach (var i in model.Intersections) {
                i.Init();
            }

            Console.WriteLine("Running simulation.");
            for (int t = 0; t < model.Duration; t++) {
                foreach (var i in model.Intersections) {
                    if (i.TrafficLights.Count == 0) continue;
                    i.SwitchTrafficLight();
                }
            }

            Console.WriteLine("Simulation completed");
        }
    }
}
