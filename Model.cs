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
    }

    public class TrafficLight {
        public Street Street;
        public int GreenDuration;
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