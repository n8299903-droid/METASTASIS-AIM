package dev.seraph.metastasis;

import java.util.List;

public final class Main {
    private Main() {}

    public record Sample(long timestampMillis, double yaw, double pitch) {}
    public record Finding(long timestampMillis, double degreesPerSecond, String severity) {}

    public static List<Finding> analyze(List<Sample> samples, double reviewThreshold) {
        if (samples.size() < 2) return List.of();

        var findings = new java.util.ArrayList<Finding>();
        for (int i = 1; i < samples.size(); i++) {
            Sample previous = samples.get(i - 1);
            Sample current = samples.get(i);
            long elapsed = current.timestampMillis() - previous.timestampMillis();
            if (elapsed <= 0) continue;

            double yawDelta = shortestAngle(previous.yaw(), current.yaw());
            double pitchDelta = current.pitch() - previous.pitch();
            double distance = Math.hypot(yawDelta, pitchDelta);
            double speed = distance * 1000.0 / elapsed;

            if (speed >= reviewThreshold) {
                String severity = speed >= reviewThreshold * 1.8 ? "HIGH" : "REVIEW";
                findings.add(new Finding(current.timestampMillis(), speed, severity));
            }
        }
        return List.copyOf(findings);
    }

    private static double shortestAngle(double from, double to) {
        double delta = (to - from) % 360.0;
        if (delta > 180.0) delta -= 360.0;
        if (delta < -180.0) delta += 360.0;
        return delta;
    }

    public static void main(String[] args) {
        // Synthetic demonstration data only; no game client or player input is accessed.
        List<Sample> demo = List.of(
            new Sample(0, 10.0, 2.0),
            new Sample(100, 12.0, 2.5),
            new Sample(200, 14.0, 3.0),
            new Sample(250, 82.0, 8.0),
            new Sample(350, 84.0, 8.5)
        );

        System.out.println("METASTASIS-AIM | Offline telemetry demo");
        System.out.println("Review threshold: 360 degrees/second");
        List<Finding> findings = analyze(demo, 360.0);
        if (findings.isEmpty()) {
            System.out.println("No samples exceeded the review threshold.");
        } else {
            findings.forEach(f -> System.out.printf(
                "%s | t=%d ms | %.1f deg/s%n",
                f.severity(), f.timestampMillis(), f.degreesPerSecond()
            ));
        }
        System.out.println("Results are heuristic indicators, not proof of cheating.");
    }
}
