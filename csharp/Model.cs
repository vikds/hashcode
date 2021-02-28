using System.Collections.Generic;
using System.Linq;

namespace HashCode {
    public class Model {
        public Model(int duration, int intersectionsNum, int streetsNum, int carsNum, int bonus) {
            this.Duration = duration;
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
                CurrentTrafficLight.Light = TrafficLight.State.Red;
                currentTrafficLightIndex++;
                if (currentTrafficLightIndex >= TrafficLights.Count) {
                    currentTrafficLightIndex = 0;
                }
                CurrentTrafficLight.GreenSecondsLeft = CurrentTrafficLight.GreenDuration;
                CurrentTrafficLight.Light = TrafficLight.State.Green;
            }
        }
    }

    public class TrafficLight {
        public Street Street;
        public int GreenDuration;
        public State Light = State.Red;
        public int GreenSecondsLeft;

        public enum State {
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
                if (Route[currentStreetIndex].TrafficLight == null
                        || Route[currentStreetIndex].TrafficLight.Light == TrafficLight.State.Green) {
                    if (++currentStreetIndex >= Route.Length) {
                        Finished = true;
                    } else {
                        LeftOnCurrentStreet = Route[currentStreetIndex].Length - 1;
                    }
                }
            }
        }
    }
}
