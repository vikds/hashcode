using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;

namespace HashCode {
    class Simulator {
        public Simulator(Model model) {
            _model = model;
            _schedule = new Schedule(_model);
        }

        private readonly Model _model;
        private readonly Schedule _schedule;
        private readonly Dictionary<string, int> _jam = new();

        public Model RunSimulation(out long elapsedMs) {
            var simulationWatch = Stopwatch.StartNew();
            var freshModel = _model.Clone();
            _jam.Clear();

            Logger.Debug("Configuring traffic lights.");
            _schedule.ApplyToModel(freshModel);

            foreach (var i in freshModel.Intersections) {
                foreach (var t in i.TrafficLights) {
                    t.Street.TrafficLight = t;
                }
                i.SetInitialState();
            }
            foreach (var c in freshModel.Cars) {
                c.SetInitialState();
            }

            Logger.Debug("Running simulation.");
            Logger.Divider();
            for (var timer = 0; timer <= freshModel.Duration; timer++) {
                Logger.Debug($"Timer is {timer} >>>>>>>>>>>");

                // deal with traffic lights
                foreach (var intersection in freshModel.Intersections) {
                    intersection.CarJustPassed = false;
                    Logger.Debug($"Checking intersection {intersection.Id}");
                    if (intersection.TrafficLights.Count == 0) {
                        Logger.Debug($"Intersection {intersection.Id} has no traffic lights - it's always Red");
                        continue;
                    }
                    intersection.SwitchTrafficLight();
                    foreach (var tl in intersection.TrafficLights) {
                        Logger.Debug(tl.State == TrafficLight.Colors.Green
                                         ? $"Traffic light on street {tl.Street.Name} is {tl.State}. Seconds left {tl.GreenSecondsLeft}"
                                         : $"Traffic light on street {tl.Street.Name} is {tl.State}");
                    }
                }

                // deal with cars
                foreach (var car in freshModel.Cars) {
                    Logger.Debug($"Checking car {car.Id}");
                    if (car.Finished) {
                        Logger.Debug($"Car {car.Id} has already finished - skipping");
                        continue;
                    }
                    car.Move();
                    if (car.Finished) {
                        var score = freshModel.Bonus + (freshModel.Duration - timer);
                        Logger.Debug(
                            $"Car {car.Id} has just finished with score {score} = {freshModel.Bonus} + ({freshModel.Duration} - {timer})"
                        );
                        freshModel.Score += score;
                    } else {
                        Logger.Debug($"Car {car.Id} is currently on street {car.CurrentStreet.Name}. Left on current street {car.LeftOnCurrentStreet}");
                    }
                }

                // find jams
                foreach (var car in freshModel.Cars.Where(c => c.Route.Length > 0 && !c.Finished)) {
                    if (!_jam.ContainsKey(car.CurrentStreet.Name)) {
                        _jam[car.CurrentStreet.Name] = 0;
                    }
                    _jam[car.CurrentStreet.Name]++;
                }

                Logger.Debug($"Timer {timer} finished <<<<<<<<<<<");
                Logger.Divider();
            }

            simulationWatch.Stop();
            elapsedMs = simulationWatch.ElapsedMilliseconds;
            return freshModel;
        }

        public void UpdateSchedule() {
            if (_jam.Count == 0) return;

            foreach (var kvp in _jam.OrderByDescending(j => j.Value).Take(_jam.Count / 10)) {
                _schedule.DataMap[kvp.Key].GreenDuration += 1;
            }

            // var maxJamLength = int.MinValue;
            // foreach (var kvp in _jam) {
            //     maxJamLength = Math.Max(maxJamLength, kvp.Value);
            // }
            // foreach (var kvp in _jam) {
            //     if (kvp.Value == maxJamLength) {
            //         _schedule.DataMap[kvp.Key].GreenDuration *= 2;
            //     }
            // }
            // var longestJammedStreet = _jam.Aggregate((x, y) => x.Value > y.Value
            //                                              ? x
            //                                              : y).Key;
            // _schedule.DataMap[longestJammedStreet].GreenDuration *= 2;
        }
    }
}
