# Game of Graphs

## Avanced

**Functionnalities**
 - [x] links - intermediate:
 - [x] optimization:
 - [x] optimization (eval): # done probably
 - [ ] plots - conspiracies:
 - [ ] plots - parsing: # 1 / 4
 - [ ] plots - shortest paths matrix: 1 / 5

**Rigor**
 - [ ] inconsistent content
 - [ ] links- empty file
 - [ ] nonsensical argument
 - [ ] plots - empty file


Game of Graphs is a C++ program that allows you to analyze friendship relations and conspiracy intentions within a group of people. It provides two main functionalities, which can be executed using the following command line options:

## Usage

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
./game_of_graphs --links friendship_relations.txt Alice Bob
```

Analyze Conspiracy Intentions:
```shell
./game_of_graphs --plots friendship_relations.txt conspiracy.txt 5
```
