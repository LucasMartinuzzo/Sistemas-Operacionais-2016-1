import java.io.RandomAccessFile;
import java.util.logging.Level;
import java.util.logging.Logger;

//Classe relativa às threads Java.
public class ThreadArg implements Runnable {
    private Buffer buffer;

    public ThreadArg(Buffer buffer){
        this.buffer = buffer;
    }
    
    //Classe que realiza a leitura do FIFO e insere as mensagens lidas no buffer.
    public void readFifo() throws Exception{
        RandomAccessFile fifo = new RandomAccessFile("fifo","r");
        while(true){
            if(buffer.is_finished())
                break;
            String msg;
            byte[] bytes = new byte[9];
                fifo.read(bytes,0,9);
            msg = new String(bytes);
            //Ao final do envio das mensagens, as threads ainda tentam ler o FIFO
            //e acabam pegando caracteres vazios, o if seguinte ignora essas mensagens.
            if(msg.charAt(0)== ' ')
                continue;
            int idMsg = Integer.valueOf(msg.split("-")[2]);
            if(idMsg % 2 == 0){
                buffer.insert_even(msg);
            }
            else{
                buffer.insert_odd(msg);
            }
        }
        fifo.close();
    }
    
    // Sobrecarga do método executado pelas threads.
    public void run () {
        try {
            readFifo();
        } catch (Exception ex) {
            Logger.getLogger(ThreadArg.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

}
