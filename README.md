# Compiler

## Made by

    Troels Blicher Petersen (trpet15)
    Mark Jervelund          (mjerv15)
    Morten Kristian Jæger   (mojae15)

## TODO

* Implement "for" loop in grammar
* Runtime error checks
* More peephole optimization

## How to run

### Compile entire project

To make the entire project:

```bash
make
```

### Compile specific parts

To compile main program:

```bash
make compiler
```

To compile tests

```bash
make tests
```

### Run the program(s)

For compiler:

```bash
./compiler
```

Ideal run:

```bash
./compiler <filename>
```

Compile program and create executable:

```bash
./compiler -a <filename>
```

For tests:

```bash
./tests
```

## How to clean-up project

Clean only build files

```bash
make clean
```

Clean build files and compiled programs

```bash
make clean-all
```

---

## Git

How to create branch of old commit

```bash
git branch <branchname> <hash value of commit>
```

How to switch to branch 

```bash
git checkout <branchname>
```

How to delete branch

```bash
git branch -d <branchname>
```

How to list branches

```bash
git branch
```

Show previous commits in terminal

```bash
git log --stat
```

Go back n commits


```bash
git checkout HEAD-n
```

Git Bisectb

```bash
git bisect start

# Current head / commit
git bisect bad

# Last known good coomit
git bisect good <commit hash>

---->
# Tell bisect if the commit is good or bad.
git bisect good/bad

# If any changes is made during testing
git reset --hard HEAD
# - to be able to move on to another commit

---->
# Create branch from bad commit or reset.
git bisect reset / git branch <branchname> 
```