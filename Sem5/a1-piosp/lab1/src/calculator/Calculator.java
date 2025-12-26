package calculator;

import java.util.HashMap;
import java.util.Map;

public class Calculator {
    private final Map<String, Class<? extends Operation>> registry = new HashMap<>();

    public void register(String symbol, Class<? extends Operation> opClass) {
        registry.put(symbol, opClass);
    }

    public double calculate(String symbol, double... args) {
        Class<? extends Operation> opClass = registry.get(symbol);
        if (opClass == null)
            throw new IllegalArgumentException("Unsupported operation: " + symbol);

        try {
            Operation op = opClass.getDeclaredConstructor().newInstance();
            return op.execute(args);
        } catch (Exception e) {
            throw new RuntimeException("Error executing operation: " + e.getMessage());
        }
    }
}
