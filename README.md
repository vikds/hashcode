# Google HashCode just for fun

## Online Qualifications 2021

We've taken part in solving Online Qualiffications of Hash Code 2021 problem `"Traffic signaling"` for the first time. Our result was `8,446,233` points, while the best score was `10,586,135`. We would like to improve our result, and even arrange a small competition between ourselves. That's all just for fun.

## C++ requirements

1. [cmake](https://cmake.org/) >= 2.7
2. [C++17](https://en.wikipedia.org/wiki/C%2B%2B17)
3. [OMP](https://en.wikipedia.org/wiki/OpenMP) (intended to be used)

## Build C++ code

```bash
$ cd cpp
$ mkdir build
$ cd build
$ cmake ..
...
-- Build files have been written to: ~/hashcode/cpp/build
$ make
...
[100%] Built target hashcode
$ ./hashcode
Usage: ./hashcode [-io] [-i input][-o output] ...
...
```

## Run C++ code


```bash
$ ./hashcode --input=../../input/a.txt --output=../results/a.txt
```

## C++ results

|   | Name | Score |
| - | - | - |
| 1 | A – An example | 2 002 |
| 2 | B – By the ocean | 4 566 625 |
| 3 | C – Checkmate | 1 299 034 |
| 4 | D – Daily commute | 1 567 509 |
| 5 | E – Etoile | 705 971 |
| 6 | F – Forever jammed | 780 765 |

    Total: 9,482,434 points

## Building C# code
[.NET 5.0](https://dotnet.microsoft.com/download/dotnet/5.0) is required to build C# code located in `./csharp` folder. It's a completely separated version of the solution and not related in any way to a C++ code.

To build on Linux
```shell
$ cd csharp
$ dotnet publish ./HashCode.csproj \
   --output ./bin \
   --configuration Release \
   --runtime linux-x64 \
   -p:GenerateFullPaths=true \
   -p:PublishReadyToRun=true \
   -p:PublishSingleFile=true \
   --self-contained false 
```

Usage:
```shell
$ ./bin/HashCode -i ../input/a.txt -o ./output
```

Required parameters: 
- `-i|--input <input file>` - loads model from the specified file
- `-o|--output <output folder>` - puts the result here

Optional parameter:
- `-v|--verbose` - prints detailed simulation steps 


To simply compile and run C# code without publishing:
```shell
$ cd csharp
$ dotnet run -i input.txt -o ./output
```

## C# results

|   | Name | Score |
| - | - | - |
| 1 | A – An example | 2 000 |
| 2 | B – By the ocean | 4 565 916 |
| 3 | C – Checkmate | 1 242 070 |
| 4 | D – Daily commute | TBP |
| 5 | E – Etoile | 697 648 |
| 6 | F – Forever jammed | 1 196 271 |

    Total: TBP
