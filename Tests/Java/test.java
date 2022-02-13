import java.io.File;
import java.io.PrintWriter;
import java.math.BigInteger;

class Test {
    public static void main(String[] args) {
        // Declaration and initialization
        Long startingValue = Long.parseLong(args[0]);
        Long upperBound = Long.parseLong(args[1]);
        Long step = Long.parseLong(args[2]);
        PrintWriter printWriter = null;

        try {
            printWriter = new PrintWriter(new File("./JavaFactorialResults.csv"));
            printWriter.write("Value,Time Taken to Process Factorial\n");
            printWriter.flush();
        } catch (Exception e) {
            e.printStackTrace();
            return;
        }

        for (Long trial = startingValue; trial <= upperBound; trial += step) {
            // Actual calculation
            long start = System.nanoTime();
            BigInteger bigInteger = new BigInteger("1");
            for (long i = 1; i <= trial; i++) {
                bigInteger = bigInteger.multiply(BigInteger.valueOf(i));
                //COMMENT THIS TO GET MORE ACCURATE RESULTS
                //System.out.printf("\rRunning %d!:\t\t%3.2f%%",trial,(float)i/trial * 100);
            }
            long end = System.nanoTime();

            // Write the result to the file
            printWriter.println(trial + "," + (end - start));
            printWriter.flush();
            // Showing the result
            //System.out.println(trial + "! = " + bigInteger.toString());
            System.out.println("\nCalculation of " + trial + "! took " + (end - start) + " nanoSeconds");

        }

        printWriter.close();
        System.out.println("Simulation Ended");

    }
}