# METASTASIS-AIM

A standalone **aim-behavior telemetry analyzer** for controlled anti-cheat testing.

> This repository does not contain an auto-aim, target-lock, triggerbot, game-memory reader, or input injector. It does not control a Minecraft client or interact with public servers.

## What it does
- Accepts recorded yaw/pitch samples from a test harness or manually prepared CSV.
- Calculates angular movement speed and flags unusually abrupt changes for review.
- Keeps analysis offline and independent of any game process.

## Requirements
- Java 21+
- Gradle 8.14+

## Build
```bash
gradle clean build
```

The runnable JAR is created in `build/libs/`.

## Run
```bash
java -jar build/libs/metastasis-aim-1.0.0.jar
```

The demo analyzes synthetic sample data only. It is a starting point for validating anti-cheat heuristics in a private, controlled test environment.

## CSV format
The analyzer core accepts timestamp, yaw, and pitch samples in memory. A CSV importer can be added for your own authorized test recordings.

## GitHub Actions
Every push and pull request builds the project with Java 21.
