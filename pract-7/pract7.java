import java.util.*;

public class pract7 {
    static Map<Character, List<String>> grammar = new HashMap<>();
    static Map<Character, Set<Character>> firstSet = new HashMap<>(), followSet = new HashMap<>();
    static Set<Character> visitedFirst = new HashSet<>(), visitedFollow = new HashSet<>();

    static void computeFirst(char nonTerminal) {
        if (visitedFirst.contains(nonTerminal)) return;
        visitedFirst.add(nonTerminal);

        for (String production : grammar.get(nonTerminal)) {
            boolean epsilonPresent = true;

            for (char symbol : production.toCharArray()) {
                if (Character.isLowerCase(symbol) || symbol == '(' || symbol == ')') {
                    firstSet.computeIfAbsent(nonTerminal, k -> new HashSet<>()).add(symbol);
                    epsilonPresent = false;
                    break;
                } else if (Character.isUpperCase(symbol)) {
                    computeFirst(symbol);
                    boolean allEpsilon = true;

                    for (char f : firstSet.getOrDefault(symbol, new HashSet<>())) {
                        if (f != '@') firstSet.computeIfAbsent(nonTerminal, k -> new HashSet<>()).add(f);
                        if (f == '@') allEpsilon = true;
                    }

                    if (!allEpsilon) {
                        epsilonPresent = false;
                        break;
                    }
                } else if (symbol == '@') {
                    firstSet.computeIfAbsent(nonTerminal, k -> new HashSet<>()).add('@');
                }
            }

            if (!epsilonPresent) firstSet.get(nonTerminal).remove('@');
        }
    }

    static void computeFollow(char nonTerminal) {
        if (visitedFollow.contains(nonTerminal)) return;
        visitedFollow.add(nonTerminal);

        if (nonTerminal == 'S') followSet.computeIfAbsent(nonTerminal, k -> new HashSet<>()).add('$');

        for (Map.Entry<Character, List<String>> entry : grammar.entrySet()) {
            char lhs = entry.getKey();
            for (String production : entry.getValue()) {
                for (int i = 0; i < production.length(); i++) {
                    if (production.charAt(i) == nonTerminal) {
                        boolean inheritFollow = false;
                        
                        for (int j = i + 1; j < production.length(); j++) {
                            char nextSymbol = production.charAt(j);
                            if (Character.isUpperCase(nextSymbol)) {
                                followSet.computeIfAbsent(nonTerminal, k -> new HashSet<>())
                                         .addAll(firstSet.getOrDefault(nextSymbol, new HashSet<>()))
                                         ;
                                followSet.get(nonTerminal).remove('@');
                                
                                if (firstSet.getOrDefault(nextSymbol, new HashSet<>()).contains('@')) {
                                    inheritFollow = true;
                                } else {
                                    inheritFollow = false;
                                    break;
                                }
                            } else {
                                followSet.computeIfAbsent(nonTerminal, k -> new HashSet<>()).add(nextSymbol);
                                inheritFollow = false;
                                break;
                            }
                        }
                        
                        if (inheritFollow || i + 1 == production.length()) {
                            if (lhs != nonTerminal) {
                                computeFollow(lhs);
                                followSet.computeIfAbsent(nonTerminal, k -> new HashSet<>())
                                         .addAll(followSet.getOrDefault(lhs, new HashSet<>()));
                            }
                        }
                    }
                }
            }
        }
    }

    static void printFirstSet(char nonTerminal) {
        System.out.print("First(" + nonTerminal + ") = { ");
        for (char c : firstSet.getOrDefault(nonTerminal, new HashSet<>())) {
            System.out.print(c == '@' ? "ε " : c + " ");
        }
        System.out.println("}");
    }

    public static void main(String[] args) {
        grammar.put('S', Arrays.asList("ABC", "D"));
        grammar.put('A', Arrays.asList("a", "@"));
        grammar.put('B', Arrays.asList("b", "@"));
        grammar.put('C', Arrays.asList("(S)", "c"));
        grammar.put('D', Arrays.asList("AC"));

        for (char nonTerminal : grammar.keySet()) computeFirst(nonTerminal);
        for (char nonTerminal : grammar.keySet()) computeFollow(nonTerminal);

        System.out.println("First and Follow Sets:");

        printFirstSet('S');
        for (char nonTerminal : firstSet.keySet()) {
            if (nonTerminal != 'S') {
                printFirstSet(nonTerminal);
            }
        }

        for (Map.Entry<Character, Set<Character>> entry : followSet.entrySet()) {
            System.out.print("Follow(" + entry.getKey() + ") = { ");
            for (char c : entry.getValue()) {
                System.out.print(c + " ");
            }
            System.out.println("}");
        }
    }
}