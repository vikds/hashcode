using System.Collections.Generic;

namespace HashCode {
    public class Model {
        public Model() { }

        public void Init(int D, int I, int S, int V, int F) {
            Duration = D;
            Bonus = F;
            Intersections = new Intersection[I];
            for (int i = 0; i < Intersections.Length; i++) {
                Intersections[i] = new Intersection() {
                    Id = i,
                };
            }
            Streets = new Street[S];
            Vehicles = new Vehicle[V];

        }
        public int Duration;
        public Intersection[] Intersections;
        public Street[] Streets;
        public Vehicle[] Vehicles;
        public int Bonus;
    }

    public class Intersection {
        public int Id;
        public List<Street> Incoming = new List<Street>();
        public List<Street> Outcoming = new List<Street>();

        public List<TrafficLight> TrafficLights = new List<TrafficLight>();

        private int currentTrafficLightIndex = 0;

        public void Init() {
            if (TrafficLights.Count == 0) return;
            currentTrafficLightIndex = TrafficLights.Count - 1;
        }

        public void SwitchTrafficLight() {
            if (TrafficLights.Count == 0) return;

            var current = TrafficLights[currentTrafficLightIndex];
            if (current.GreenSecondsLeft > 0) {
                current.GreenSecondsLeft--;
            }
            else if (current.GreenSecondsLeft == 0) {
                current.Light = TrafficLight.State.Red;
                currentTrafficLightIndex++;
                if (currentTrafficLightIndex >= TrafficLights.Count) {
                    currentTrafficLightIndex = 0;
                }
                var newCurrent = TrafficLights[currentTrafficLightIndex];
                newCurrent.GreenSecondsLeft = newCurrent.GreenDuration;
                newCurrent.Light = TrafficLight.State.Green;
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

    public class Street {
        public string Name;
        public int Length;

        public Intersection StartsAt;
        public Intersection EndsAt;
    }

    public class Vehicle {
        public int Id;
        public Street[] Route;
    }
}