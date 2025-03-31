import java.util.*;

public class LL1Parser {

    static Map<String, Map<String, String>> parsingTable = new HashMap<>();
    static Map<String, List<String>> grammar = new HashMap<>();

    static void initializeGrammar() {
        grammar.put("E", Arrays.asList("TE'"));
        grammar.put("E'", Arrays.asList("+TE'", "ε"));
        grammar.put("T", Arrays.asList("FT'"));
        grammar.put("T'", Arrays.asList("*FT'", "ε"));
        grammar.put("F", Arrays.asList("id", "(E)"));
    }

    static void initializeParsingTable() {
        parsingTable.put("E", Map.of(
            "id", "TE'",
            "(", "TE'"
        ));
        
        parsingTable.put("E'", Map.of(
            "+", "+TE'",
            ")", "ε",
            "$", "ε"
        ));
        
        parsingTable.put("T", Map.of(
            "id", "FT'",
            "(", "FT'"
        ));
        
        parsingTable.put("T'", Map.of(
            "+", "ε",
            "*", "*FT'",
            ")", "ε",
            "$", "ε"
        ));

        parsingTable.put("F", Map.of(
            "id", "id",
            "(", "(E)"
        ));
    }

    static void printParsingTable() {
        System.out.println("\nPredictive Parsing Table:");
        System.out.printf("%-10s", "");
        Set<String> terminals = new HashSet<>(Arrays.asList("id", "+", "*", "(", ")", "$"));

        for (String terminal : terminals) {
            System.out.printf("%-10s", terminal);
        }
        System.out.println();

        for (String nonTerminal : parsingTable.keySet()) {
            System.out.printf("%-10s", nonTerminal);
            for (String terminal : terminals) {
                System.out.printf("%-10s", parsingTable.get(nonTerminal).getOrDefault(terminal, ""));
            }
            System.out.println();
        }
    }

    static void parse(String input) {
        Stack<String> stack = new Stack<>();
        stack.push("$");
        stack.push("E");
        input += "$";

        int index = 0;
        System.out.printf("\n%-15s %-15s %-15s\n", "STACK", "INPUT", "OUTPUT");

        while (!stack.isEmpty()) {
            String top = stack.peek();
            String current = String.valueOf(input.charAt(index));

            System.out.printf("%-15s %-15s", stack, input.substring(index));

            if (top.equals(current)) {
                stack.pop();
                index++;
                System.out.println("Match " + current);
            } else if (parsingTable.containsKey(top)) {
                if (parsingTable.get(top).containsKey(current)) {
                    String production = parsingTable.get(top).get(current);
                    System.out.println(top + " → " + production);
                    stack.pop();
                    if (!production.equals("ε")) {
                        for (int i = production.length() - 1; i >= 0; i--) {
                            stack.push(String.valueOf(production.charAt(i)));
                        }
                    }
                } else {
                    System.out.println("Error: No rule for " + top + " with input " + current);
                    return;
                }
            } else {
                System.out.println("Error: Invalid symbol " + top);
                return;
            }
        }

        System.out.println("\nString is successfully parsed.");
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        initializeGrammar();
        initializeParsingTable();
        printParsingTable();

        System.out.print("Enter input string: ");
        String input = scanner.next();

        parse(input);
    }
}
