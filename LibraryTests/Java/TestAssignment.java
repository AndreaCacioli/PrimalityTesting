import java.io.File;
import java.io.PrintWriter;
import java.math.BigInteger;
import java.util.Random;

class TestAssignment {
    public static void main(String[] args) {
        // Declaration and initialization
        Long startingValue = Long.parseLong(args[0]);
        Long upperBound = Long.parseLong(args[1]);
        Long step = Long.parseLong(args[2]);
        PrintWriter printWriter = null;

        try {
            printWriter = new PrintWriter(new File("./JavaAssignmentResults.csv"));
            printWriter.write("Value,Time Taken to Assign Values (Java)\n");
            printWriter.flush();
        } catch (Exception e) {
            e.printStackTrace();
            return;
        }

        Random random = new Random();
        Character[] numbers = new Character[]{'0','1','2','3','4','5','6','7','8','9'};

        for (Long trial = startingValue; trial <= upperBound; trial += step) {

            String str = new String();
            for(long i = 0 ; i < trial; i++)
            {
                str += numbers[random.nextInt(numbers.length)];
            }

            // Actual calculation
            long start = System.nanoTime();
            BigInteger bigInteger = new BigInteger(str);
            long end = System.nanoTime();

            // Write the result to the file
            printWriter.println(trial + "," + (end - start) / 1000);
            printWriter.flush();
            // Showing the result
            //System.out.println(trial + "! = " + bigInteger.toString());
            System.out.println("\nAssignment of a random " + trial + " characters long number took " + (end - start) / 1000 + " microSeconds");
        }

        printWriter.close();
        System.out.println("Simulation Ended");

    }
}