# Graphanalysis

Graphanalysis is a C++ program that allows you to analyze friendship relations and conspiracy intentions within a group of people.
The goal of this project is to handle **graphs**, **recursive algorithms**, **matrix operations**...

It provides two main functionalities, which can be executed using the following command line options :

```shell
./game_of_graphs [--links fr p1 p2 | --plots fr cr n]
```

## Command Line Options
- --links fr p1 p2: Analyze friendship relations between two individuals (p1 and p2) in the friendships file (fr) and find the shortest path between them, if it exists.

- --plots fr cr n: Explore conspiracy intentions by analyzing friendship relations from the friendships file (fr) and conspiracy intentions from the conspiracy intentions file (cr) while considering a maximum path length of n.

## Arguments
- fr: File containing friendship relations between people.

- p1: Name of the first person in the friendships file.

- p2: Name of the second person in the friendships file.

- cr: File containing conspiracy intentions.

- n: Maximum length of friendship paths to consider in conspiracy analysis.

## Example Usage
Analyze Friendship Relations:
```shell
./game_of_graphs --links friendship_files/friends_example "Jon Snow" "Theon Greyjoy"

-> Degree of separation between Jon Snow and Theon Greyjoy: 1
```

Analyze Conspiracy Intentions:
```shell
./game_of_graphs --plots friendship_files/friends_example plots_files/plots_example 2

-> Names:
Cersei Lannister
Daenerys Targaryen
Jeor Mormont
Jon Snow
Jorah Mormont
Lynce Hightower
Maester Aemon
Missandei
Samwell Tarly
Sansa Stark
The Mountain
Theon Greyjoy
Tyrion Lannister
Tywin Lannister
Vere

Relationships:
0 2 2 2 1 2 2 0 2 1 1 0 1 1 0
2 0 2 0 1 2 0 1 0 0 0 0 0 0 0
2 2 0 0 1 2 0 0 0 0 0 0 0 0 0
2 0 0 0 0 0 2 0 1 1 0 1 0 0 2
1 1 1 0 0 1 0 2 0 2 2 0 2 2 0
2 2 2 0 1 0 0 0 0 0 0 0 0 0 0
2 0 0 2 0 0 0 0 1 2 0 0 0 1 2
0 1 0 0 2 0 0 0 0 0 0 0 0 0 0
2 0 0 1 0 0 1 0 0 1 0 2 0 2 1
1 0 0 1 2 0 2 0 1 0 2 2 2 2 2
1 0 0 0 2 0 0 0 0 2 0 0 2 2 0
0 0 0 1 0 0 0 0 2 2 0 0 0 0 0
1 0 0 0 2 0 0 0 0 2 2 0 0 2 0
1 0 0 0 2 0 1 0 2 2 2 0 2 0 0
0 0 0 2 0 0 2 0 1 2 0 0 0 0 0

Plots:
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 1 0 1 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
1 0 0 0 0 0 0 0 0 0 0 1 0 0 0
0 0 0 1 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 1 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
1 1 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0

Conspiracies:
Jon Snow -> Tywin Lannister
Tywin Lannister -> Daenerys Targaryen
The Mountain -> Jon Snow -> Theon Greyjoy -> Sansa Stark

Result:
The Crown is safe !
```

The `--plots` option, as we can see in the example above, outputs:
- the names of the people in the friendships file
- the friendship matrix
- the conspiracy matrix
- the result of the conspiracy analysis.


### The subject

You need to build a graph representing the conspiracies between each political figures (who plots against
whom). Using this graph, you will need to stop people plotting against the Queen (called direct enemies),
by plotting against them.

To do so, you need to contact every “close” friend to the Crown that is plotting against these enemies.

If an enemy has only non-close friends plotting against them, you can still convince these non-close friends by
plotting against their enemies.

You will need to check who’s plotting against them. These people are “indirect enemies”, and just like your
direct enemies, you can plot with their enemies if the latters are “close” friends. If you can plot against at
least one of their enemies, then this “non-close friend” will help you, just like a “close friend”.
You must continue until finding a way to plot (directly or indirectly) against all your direct enemies

A person is considered a “close friend” if their friendship level relative to the Queen is less than or equal to n.