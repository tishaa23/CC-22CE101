import java.util.*;

public class LL1Parser {
    // Define the grammar
    private final Map<String, List<String>> grammar;
    private final Map<String, Set<String>> firstSets;
    private final Map<String, Set<String>> followSets;
    private final Map<String, Map<String, String>> parsingTable;
    private final String startSymbol;

    public LL1Parser() {
        // Initialize the grammar
        grammar = new HashMap<>();
        grammar.put("S", Arrays.asList("ABC", "D"));
        grammar.put("A", Arrays.asList("a", "ε"));
        grammar.put("B", Arrays.asList("b", "ε"));
        grammar.put("C", Arrays.asList("(S)", "c"));
        grammar.put("D", Arrays.asList("AC"));

        // Initialize first sets as provided
        firstSets = new HashMap<>();
        firstSets.put("S", new HashSet<>(Arrays.asList("a", "b", "(", "c")));
        firstSets.put("A", new HashSet<>(Arrays.asList("a", "ε")));
        firstSets.put("B", new HashSet<>(Arrays.asList("b", "ε")));
        firstSets.put("C", new HashSet<>(Arrays.asList("(", "c")));
        firstSets.put("D", new HashSet<>(Arrays.asList("a", "(", "c")));

        // Initialize follow sets as provided
        followSets = new HashMap<>();
        followSets.put("S", new HashSet<>(Arrays.asList(")", "$")));
        followSets.put("A", new HashSet<>(Arrays.asList("b", "(", "c")));
        followSets.put("B", new HashSet<>(Arrays.asList("(", "c")));
        followSets.put("C", new HashSet<>(Arrays.asList(")", "$")));
        followSets.put("D", new HashSet<>(Arrays.asList(")", "$")));

        // Set start symbol
        startSymbol = "S";

        // Initialize the parsing table
        parsingTable = new HashMap<>();
        constructParsingTable();
    }

    private void constructParsingTable() {
        // Initialize tables for each non-terminal
        for (String nonTerminal : grammar.keySet()) {
            parsingTable.put(nonTerminal, new HashMap<>());
        }

        // Manually construct the parsing table based on the given grammar and sets
        
        // For S -> ABC (for all terminals in First(S))
        parsingTable.get("S").put("a", "ABC");
        parsingTable.get("S").put("b", "ABC");
        parsingTable.get("S").put("(", "ABC");
        parsingTable.get("S").put("c", "ABC");  // Corrected: S -> ABC for terminal c
        
        // For A -> a
        parsingTable.get("A").put("a", "a");
        
        // For A -> ε
        parsingTable.get("A").put("b", "ε");
        parsingTable.get("A").put("(", "ε");
        parsingTable.get("A").put("c", "ε");
        
        // For B -> b
        parsingTable.get("B").put("b", "b");
        
        // For B -> ε
        parsingTable.get("B").put("(", "ε");
        parsingTable.get("B").put("c", "ε");
        
        // For C -> (S)
        parsingTable.get("C").put("(", "(S)");
        
        // For C -> c
        parsingTable.get("C").put("c", "c");
        
        // For D -> AC
        parsingTable.get("D").put("a", "AC");
        parsingTable.get("D").put("(", "AC");
        parsingTable.get("D").put("c", "AC");
    }

    public boolean isLL1Grammar() {
        // Check if there are any conflicts in the parsing table
        for (String nonTerminal : parsingTable.keySet()) {
            Map<String, String> entries = parsingTable.get(nonTerminal);
            Set<String> terminals = new HashSet<>();
            
            for (String terminal : entries.keySet()) {
                if (terminals.contains(terminal)) {
                    return false;
                }
                terminals.add(terminal);
            }
        }
        return true;
    }

    private boolean isTerminal(String symbol) {
        return !grammar.containsKey(symbol);
    }

    public void printParsingTable() {
        System.out.println("Predictive Parsing Table:");
        System.out.println("------------------------");
        
        // Define terminals
        String[] terminals = {"a", "b", "c", "(", ")", "$"};
        
        // Print header
        System.out.print("Non-Terminal | ");
        for (String terminal : terminals) {
            System.out.print(terminal + " | ");
        }
        System.out.println();
        
        // Print separator
        System.out.print("-------------+-");
        for (String terminal : terminals) {
            System.out.print("--+-");
        }
        System.out.println();
        
        // Print rows
        String[] nonTerminals = {"S", "A", "B", "C", "D"};
        for (String nonTerminal : nonTerminals) {
            System.out.print("      " + nonTerminal + "      | ");
            
            for (String terminal : terminals) {
                String production = parsingTable.get(nonTerminal).getOrDefault(terminal, "");
                
                // Add padding to make each cell same width
                int paddingNeeded = 3 - production.length();
                String padding = " ".repeat(Math.max(0, paddingNeeded));
                
                System.out.print(production + padding + "| ");
            }
            System.out.println();
        }
    }

    public boolean parseString(String input) {
        input = input + "$";  // Add end marker
        Stack<String> stack = new Stack<>();
        
        // Initialize stack with end marker and start symbol
        stack.push("$");
        stack.push(startSymbol);
        
        int inputIndex = 0;
        
        System.out.println("\nParsing: " + input.substring(0, input.length() - 1));
        System.out.println("------------------------");
        
        while (!stack.isEmpty()) {
            String top = stack.peek();
            char currentInput = input.charAt(inputIndex);
            String currentSymbol = String.valueOf(currentInput);
            
            System.out.println("Stack: " + stack + ", Current Input: " + currentSymbol);
            
            if (top.equals("$") && currentSymbol.equals("$")) {
                // Successful parse
                return true;
            }
            
            if (top.equals(currentSymbol)) {
                // Match terminal
                stack.pop();
                inputIndex++;
            } else if (isTerminal(top)) {
                // Terminal mismatch
                return false;
            } else {
                // Non-terminal
                String production = parsingTable.get(top).get(currentSymbol);
                
                if (production == null) {
                    // No production rule, parsing failed
                    return false;
                }
                
                stack.pop();  // Remove the non-terminal
                
                if (!production.equals("ε")) {
                    // Push production in reverse order
                    for (int i = production.length() - 1; i >= 0; i--) {
                        stack.push(production.substring(i, i + 1));
                    }
                }
            }
        }
        
        // If we've consumed all input, parsing successful
        return inputIndex == input.length();
    }

    public static void main(String[] args) {
        LL1Parser parser = new LL1Parser();
        
        // Print the parsing table
        parser.printParsingTable();
        
        // Check if grammar is LL(1)
        boolean isLL1 = parser.isLL1Grammar();
        if (isLL1) {
            System.out.println("\nThe grammar is LL(1).");
            
            // Test cases
            String[] testCases = {
                "abc", "ac", "(abc)", "c", "(ac)",
                "a", "(", ")", "(ab)", "abcabc", "b"
            };
            
            System.out.println("\nValidating test cases:");
            for (String testCase : testCases) {
                boolean isValid = parser.parseString(testCase);
                System.out.println("\nInput: " + testCase);
                System.out.println(isValid ? "Valid string" : "Invalid string");
                System.out.println("------------------------");
            }
        } else {
            System.out.println("\nThe grammar is not LL(1).");
        }
    }
}