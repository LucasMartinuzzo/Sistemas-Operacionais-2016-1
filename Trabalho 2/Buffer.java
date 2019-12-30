//Classe que contém os dois buffers nos quais serão armazenadas as mensagens
//lidas pelas threads Java e algumas informações relativas a eles.
public class Buffer {
    private final static int TAM_BUFFER = 500;
    private static String[] bufferEven = new String[TAM_BUFFER];
    private static String[] bufferOdd = new String [TAM_BUFFER];
    private static int evenIndex = 0;
    private static int oddIndex = 0;
    private static Boolean evenFull = false;
    private static Boolean oddFull = false;

    //Getters de atributos da classe (tamanho máximo dos buffers e indíces atuais
    //de cada um deles).
    public static int get_tam(){
        return TAM_BUFFER;
    }

    public static int get_even_index(){
        return evenIndex;
    }

    public static int get_odd_index(){
        return oddIndex;
    }

    //Métodos de inserção das mensagens lidas nos buffers, as quais incrementam
    //os seus respectivos indíces e indicam caso eles estejam cheios.
    public synchronized static void insert_even(String msg){
        bufferEven[evenIndex] = msg;
        evenIndex++;
        if(evenIndex == TAM_BUFFER)
            evenFull = true;
    }

    public synchronized static void insert_odd(String msg){
        bufferOdd[oddIndex] = msg;
        oddIndex++;
        if(oddIndex == TAM_BUFFER)
            oddFull = true;
    }

    //Métodos que verificam se a comunicação entre as threads foi encerrada, ou
    //seja, se os dois buffers estão cheios.
    public static Boolean is_finished(){
        if(oddFull && evenFull)
            return true;
        else
            return false;
    }
    
    //Métodos de impressão dos buffers.
    public static void imprimir_par(){
        System.out.println("Buffer par: ");
        for(int i = 0;i<TAM_BUFFER;i++)
            System.out.println(bufferEven[i]);
    }

    
    public static void imprimir_impar(){
        System.out.println("\nBuffer ímpar: ");
        for(int i = 0;i<TAM_BUFFER;i++)
            System.out.println(bufferOdd[i]);
    }
}
