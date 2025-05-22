import java.io.*;

class OS {
    // Core data structures for Operating System
    private char[][] memory = new char[100][4];
    private char[] buffer = new char[40];
    private char[] R = new char[4];
    private char[] IR = new char[4];
    private int IC;
    private int T;
    private int SI;

    // Non-core data structures
    private int memory_used;
    private String input_file;
    private String output_file;
    private BufferedReader fread;
    private BufferedWriter fwrite;

    public OS(String file, String output) {
        this.input_file = file;
        this.output_file = output;
        this.SI = 0;
        try {
            this.fread = new BufferedReader(new FileReader(file));
            this.fwrite = new BufferedWriter(new FileWriter(output));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void LOAD() {
        String line;
        try {
            while ((line = fread.readLine()) != null) {
                buffer = line.toCharArray();

                if (buffer[0] == '$' && buffer[1] == 'A' && buffer[2] == 'M' && buffer[3] == 'J') {
                    System.out.println("PROGRAM CARD");
                    init();
                    continue;
                } else if (buffer[0] == '$' && buffer[1] == 'D' && buffer[2] == 'T' && buffer[3] == 'A') {
                    System.out.println("DATA CARD");
                    execute();
                    continue;
                } else if (buffer[0] == '$' && buffer[1] == 'E' && buffer[2] == 'N' && buffer[3] == 'D') {
                    System.out.println("END CARD");
                    print_memory();
                    continue;
                }

                if (memory_used == 100) {
                    System.out.println("Abort due to exceeding memory usage");
                    break;
                }

                for (int i = 0; i < line.length(); i++) {
                    memory[memory_used][i % 4] = buffer[i];
                    if ((i + 1) % 4 == 0) memory_used++;
                }
            }
            fwrite.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void execute() {
        while (IC < 100) {
            System.arraycopy(memory[IC], 0, IR, 0, 4);
            IC++;
            String instruction = new String(IR).trim();

            if (instruction.startsWith("LR")) loadRegister(instruction);
            else if (instruction.startsWith("SR")) storeRegister(instruction);
            else if (instruction.startsWith("CR")) compareRegister(instruction);
            else if (instruction.startsWith("BT")) branchIfTrue(instruction);
            else if (instruction.startsWith("GD")) { SI = 1; masterMode(); }
            else if (instruction.startsWith("PD")) { SI = 2; masterMode(); }
            else if (instruction.startsWith("H")) { SI = 3; masterMode(); break; }
        }
    }

    private void loadRegister(String instruction) {
        int address = Integer.parseInt(instruction.substring(2));
        System.arraycopy(memory[address], 0, R, 0, 4);
    }

    private void storeRegister(String instruction) {
        int address = Integer.parseInt(instruction.substring(2));
        System.arraycopy(R, 0, memory[address], 0, 4);
    }

    private void compareRegister(String instruction) {
        int address = Integer.parseInt(instruction.substring(2));
        T = (new String(memory[address]).equals(new String(R))) ? 1 : 0;
    }

    private void branchIfTrue(String instruction) {
        if (T == 1) IC = Integer.parseInt(instruction.substring(2));
    }

    private void masterMode() {
        try {
            if (SI == 1) Read();
            else if (SI == 2) Write();
            else if (SI == 3) fwrite.write("\n\n");
            SI = 0;
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void Write() {
        int address = Integer.parseInt(new String(IR).substring(2));
        StringBuilder output = new StringBuilder();

        for (int i = 0; i < 10; i++) {
            if (memory[address + i][0] != '~') {  // Avoid printing empty memory
                output.append(new String(memory[address + i]).trim());
            }
        }

        if (!output.toString().isEmpty()) {
            System.out.println(output + " -- Output --");
            try {
                fwrite.write(output.toString());
                fwrite.write("\n");
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private void Read() {
        int address = Integer.parseInt(new String(IR).substring(2));
        try {
            String line = fread.readLine();
            if (line != null) {
                buffer = line.toCharArray();
                for (int i = 0; i < buffer.length; i++) {
                    memory[address][i % 4] = buffer[i];
                    if ((i + 1) % 4 == 0) address++;
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void init() {
        memory_used = 0;
        T = 0;
        IC = 0;
        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 4; j++) {
                memory[i][j] = '~';  // Fill memory with '~' to indicate uninitialized space
            }
        }
    }

    public void print_memory() {
        for (int i = 0; i < 100; i++) {
            String row = new String(memory[i]).replace("~~~~", "").trim();
            if (!row.isEmpty()) {
                System.out.println("[" + i + "] --> " + row);
            }
        }
    }
}

public class Main {
    public static void main(String[] args) {
        OS u = new OS("C:\\Users\\ajaga\\OneDrive\\Desktop\\OS\\CP\\OS-CP\\src\\input.txt", "output.txt");
        u.LOAD();
    }
}
