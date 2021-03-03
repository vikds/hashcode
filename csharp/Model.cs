using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HashCode {
    public class Model {
        public Model(int duration, int intersectionsNum, int streetsNum, int carsNum, int bonus) {
            Duration = duration;
            Bonus = bonus;
            Intersections = new Intersection[intersectionsNum];
            for (int i = 0; i < Intersections.Length; i++) {
                Intersections[i] = new Intersection() {
                    Id = i,
                };
            }
            Streets = new Street[streetsNum];
            Cars = new Car[carsNum];

        }
        public int Duration;
        public Intersection[] Intersections;
        public Street[] Streets;
        public Car[] Cars;
        public int Bonus;
        public int Score;

        public string ExportSchedule() {
            var sb = new StringBuilder();
            var intersections = Intersections
                    .Where(i => i.TrafficLights != null && i.TrafficLights.Count > 0)
                    .ToArray();

            sb.Append(intersections.Length.ToString());
            foreach (var i in intersections) {
                sb.AppendLine();
                sb.AppendLine(i.Id.ToString());
                sb.Append(i.TrafficLights.Count.ToString());
                foreach (var t in i.TrafficLights) {
                    sb.AppendLine();
                    sb.Append($"{t.Street.Name} {t.GreenDuration}");
                }
            }
            return sb.ToString();
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
        public List<Street> Incoming = new List<Street>();
        public List<Street> Outcoming = new List<Street>();

        public List<TrafficLight> TrafficLights = new List<TrafficLight>();

        public bool CarJustPassed = false;

        private int currentTrafficLightIndex = 0;
        public TrafficLight CurrentTrafficLight {
            get {
                return TrafficLights[currentTrafficLightIndex];
            }
        }

        public void SetInitialState() {
            if (TrafficLights.Count == 0) return;
            currentTrafficLightIndex = TrafficLights.Count - 1;
        }

        public void SwitchTrafficLight() {
            if (TrafficLights.Count == 0) return;

            if (CurrentTrafficLight.GreenSecondsLeft > 0) {
                CurrentTrafficLight.GreenSecondsLeft--;
            }
            if (CurrentTrafficLight.GreenSecondsLeft == 0) {
                CurrentTrafficLight.State = TrafficLight.Colors.Red;
                currentTrafficLightIndex++;
                if (currentTrafficLightIndex >= TrafficLights.Count) {
                    currentTrafficLightIndex = 0;
                }
                CurrentTrafficLight.GreenSecondsLeft = CurrentTrafficLight.GreenDuration;
                CurrentTrafficLight.State = TrafficLight.Colors.Green;
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

        public void Move() {
            if (LeftOnCurrentStreet > 0) {
                LeftOnCurrentStreet--;
            } else { // if (leftOnCurrentStreet == 0)
                if (CurrentStreet.EndsAt.CarJustPassed) {
                    Logger.Log(
                        "Car {0} is waiting in the line at intersection {1}",
                        Id,
                        CurrentStreet.EndsAt.Id
                    );
                } else if (CurrentStreet.TrafficLight != null
                        && CurrentStreet.TrafficLight.State == TrafficLight.Colors.Red) {
                    Logger.Log(
                        "Car {0} is waiting for green light at intersection {1}",
                        Id,
                        CurrentStreet.EndsAt.Id
                    );
                }
                if (!CurrentStreet.EndsAt.CarJustPassed
                        && (CurrentStreet.TrafficLight == null
                        || CurrentStreet.TrafficLight.State == TrafficLight.Colors.Green)) {
                    if (++currentStreetIndex >= Route.Length) {
                        Logger.Log("Car {0} successfully finished", Id);
                        Finished = true;
                    } else {
                        CurrentStreet.StartsAt.CarJustPassed = true;
                        LeftOnCurrentStreet = CurrentStreet.Length - 1;
                    }
                }
            }
        }
    }
}
