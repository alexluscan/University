package calculator;

import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Calculator calc = new Calculator();

        calc.register("+", AddOp.class);
        calc.register("-" , SubOp.class);
        calc.register("*", MulOp.class);
        calc.register("/", DivOp.class);
        calc.register("min", MinOp.class);
        calc.register("max", MaxOp.class);
        calc.register("sqrt", SqrtOp.class);

        System.out.println("Calculator");
        System.out.println("Supported: +, -, *, /, min, max, sqrt");
        System.out.println("Type 'exit' to quit\n");

        Scanner sc = new Scanner(System.in);

        while (true) {
            System.out.print("Enter: ");
            String line = sc.nextLine().trim();
            if (line.equalsIgnoreCase("exit")) break;

            try {
                String[] parts = line.split("\\s+");
                String symbol = parts[0];
                double[] nums = new double[parts.length - 1];
                for (int i = 1; i < parts.length; i++)
                    nums[i - 1] = Double.parseDouble(parts[i]);
                double result = calc.calculate(symbol, nums);
                System.out.println("Result: " + result + "\n");
            } catch (Exception e) {
                System.out.println("Error: " + e.getMessage() + "\n");
            }
        }

        sc.close();
    }
}
