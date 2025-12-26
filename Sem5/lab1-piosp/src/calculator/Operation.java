package calculator;

public interface Operation {
    double execute(double... args);
}

class AddOp implements Operation {
    public double execute(double... args) {
        return args[0] + args[1];
    }
}

class SubOp implements Operation {
    public double execute(double... args) {
        return args[0] - args[1];
    }
}

class MulOp implements Operation {
    public double execute(double... args) {
        return args[0] * args[1];
    }
}

class DivOp implements Operation {
    public double execute(double... args) {
        if (args[1] == 0) throw new IllegalArgumentException("Division by zero");
        return args[0] / args[1];
    }
}

class MinOp implements Operation {
    public double execute(double... args) {
        double min = args[0];
        for (double v : args) if (v < min) min = v;
        return min;
    }
}

class MaxOp implements Operation {
    public double execute(double... args) {
        double max = args[0];
        for (double v : args) if (v > max) max = v;
        return max;
    }
}

class SqrtOp implements Operation {
    public double execute(double... args) {
        if (args[0] < 0) throw new IllegalArgumentException("Negative input for sqrt");
        return Math.sqrt(args[0]);
    }
}
