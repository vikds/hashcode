using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HashCode {
    public class Model {
        public Model() { }
        public Model(int duration, int intersectionsNum, int streetsNum, int carsNum, int bonus) {
            Duration = duration;
            Bonus = bonus;
            Intersections = new Intersection[intersectionsNum];
            for (var i = 0; i < Intersections.Length; i++) {
                Intersections[i] = new Intersection() {
                    Id = i,
                    Incoming = new List<Street>(streetsNum),
                    Outcoming = new List<Street>(streetsNum),
                };
            }
            Streets = new Street[streetsNum];
            Cars = new Car[carsNum];

        }

        public readonly int Duration;
        public readonly Intersection[] Intersections;
        public readonly Street[] Streets;
        public readonly Dictionary<string, Street> StreetMap = new();
        public readonly Car[] Cars;
        public readonly int Bonus;
        public int Score;

        public string ExportSchedule() {
            var sb = new StringBuilder();
            var intersections = Intersections
                .Where(i => i.TrafficLights != null && i.TrafficLights.Count > 0)
                .ToArray();

            sb.Append(intersections.Length);
            foreach (var i in intersections) {
                sb.AppendLine();
                sb.AppendLine(i.Id.ToString());
                sb.Append(i.TrafficLights.Count);
                foreach (var t in i.TrafficLights) {
                    sb.AppendLine();
                    sb.Append($"{t.Street.Name} {t.GreenDuration}");
                }
            }
            return sb.ToString();
        }

        public Model Clone() {
            var clone = new Model(
                duration: Duration,
                intersectionsNum: Intersections.Length,
                streetsNum: Streets.Length,
                carsNum: Cars.Length,
                bonus: Bonus
            );

            for (var i = 0; i < clone.Streets.Length; i++) {
                var street = Streets[i];
                var clonedStreet = new Street() {
                    StartsAt = clone.Intersections[street.StartsAt.Id],
                    EndsAt = clone.Intersections[street.EndsAt.Id],
                    Name = street.Name,
                    Length = street.Length,
                };
                clone.StreetMap[clonedStreet.Name] = clonedStreet;
                clonedStreet.StartsAt.Outcoming.Add(clonedStreet);
                clonedStreet.EndsAt.Incoming.Add(clonedStreet);
                clone.Streets[i] = clonedStreet;
            }

            for (var i = 0; i < clone.Cars.Length; i++) {
                var originalCar = Cars[i];
                var clonedCar = new Car() {
                    Id = i,
                    Route = new Street[originalCar.Route.Length],
                };
                for (var j = 0; j < originalCar.Route.Length; j++) {
                    clonedCar.Route[j] = clone.StreetMap[originalCar.Route[j].Name];
                }
                clone.Cars[i] = clonedCar;
            }
            return clone;
        }
    }

    public class Street {
        public string Name;
        public int Length;

        public Intersection StartsAt;
        public Intersection EndsAt;

        public TrafficLight TrafficLight;
    }

    public class Intersection {
        public int Id;
        public List<Street> Incoming = new();
        public List<Street> Outcoming = new();

        public readonly List<TrafficLight> TrafficLights = new();

        public bool CarJustPassed = false;

        private int _currentTrafficLightIndex = 0;
        public TrafficLight CurrentTrafficLight => TrafficLights[_currentTrafficLightIndex];

        public void SetInitialState() {
            if (TrafficLights.Count == 0) return;
            _currentTrafficLightIndex = TrafficLights.Count - 1;
        }

        public void SwitchTrafficLight() {
            if (TrafficLights.Count == 0) return;
            var currentTrafficLight = TrafficLights[_currentTrafficLightIndex];
            if (currentTrafficLight.GreenSecondsLeft > 0) {
                currentTrafficLight.GreenSecondsLeft--;
            }
            if (currentTrafficLight.GreenSecondsLeft == 0) {
                currentTrafficLight.State = TrafficLight.Colors.Red;
                _currentTrafficLightIndex++;
                if (_currentTrafficLightIndex >= TrafficLights.Count) {
                    _currentTrafficLightIndex = 0;
                }
                currentTrafficLight = TrafficLights[_currentTrafficLightIndex];
                currentTrafficLight.GreenSecondsLeft = currentTrafficLight.GreenDuration;
                currentTrafficLight.State = TrafficLight.Colors.Green;
            }
        }
    }

    public partial class TrafficLight {
        public Street Street;
        public int GreenDuration;

        public Colors State = Colors.Red;
        public int GreenSecondsLeft;

        public enum Colors {
            Green,
            Red
        }
    }

    public class Car {
        public int Id;
        public Street[] Route;

        public bool Finished { get; private set; }
        public int LeftOnCurrentStreet;

        private int currentStreetIndex;
        public Street CurrentStreet {
            get {
                return Route[currentStreetIndex];
            }
        }

        public void SetInitialState() {
            currentStreetIndex = 0;
            LeftOnCurrentStreet = 0;
            Finished = false;
        }

        public MoveResult Move() {
            if (Route.Length == 0) return MoveResult.NA;
            if (LeftOnCurrentStreet > 0) {
                LeftOnCurrentStreet--;
                return MoveResult.StreetMovement;
            }
            // elseif (leftOnCurrentStreet == 0)
            if (CurrentStreet.EndsAt.CarJustPassed) {
                Logger.Debug(
                    "Car {0} is waiting in the line at intersection {1}",
                    Id,
                    CurrentStreet.EndsAt.Id
                );
                return MoveResult.WaitingInTheLine;
            } else if (CurrentStreet.TrafficLight == null ||
                (CurrentStreet.TrafficLight != null
                    && CurrentStreet.TrafficLight.State == TrafficLight.Colors.Red)) {
                Logger.Debug(
                    "Car {0} is waiting for green light at intersection {1}",
                    Id,
                    CurrentStreet.EndsAt.Id
                );
                return MoveResult.WaitingAtRed;
            } else if (!CurrentStreet.EndsAt.CarJustPassed
                && (CurrentStreet.TrafficLight == null
                    || CurrentStreet.TrafficLight.State == TrafficLight.Colors.Green)) {
                if (++currentStreetIndex >= Route.Length) {
                    Logger.Debug("Car {0} successfully finished", Id);
                    Finished = true;
                    return MoveResult.JustFinished;
                } else {
                    CurrentStreet.StartsAt.CarJustPassed = true;
                    LeftOnCurrentStreet = CurrentStreet.Length - 1;
                    return MoveResult.PassedOnGreen;
                }
            }
            throw new Exception("We should not be here");
        }

        public enum MoveResult {
            StreetMovement,
            WaitingInTheLine,
            WaitingAtRed,
            PassedOnGreen,
            JustFinished,
            NA,
        }
    }
}
