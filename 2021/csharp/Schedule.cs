using System.Collections.Generic;

namespace HashCode {
    public class Schedule {
        public class Record {
            public string StreetName;
            public int GreenDuration;
        }

        public Schedule(Model model) {
            Data = new List<Record>(model.Streets.Length);
            foreach (var i in model.Intersections) {
                foreach (var s in i.Incoming) {
                    var record = new Record() {
                        StreetName = s.Name,
                        GreenDuration = 1,
                    };
                    Data.Add(record);
                    DataMap[record.StreetName] = record;
                }
            }
        }

        public List<Record> Data;
        public Dictionary<string, Record> DataMap = new();

        public void ApplyToModel(Model model) {
            foreach (var record in Data) {
                var street = model.StreetMap[record.StreetName];
                street.EndsAt.TrafficLights.Add(new TrafficLight() {
                    Street = street,
                    GreenDuration = record.GreenDuration,
                });
            }
        }
    }
}
