
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.RandomAccessFile;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author martinuzzo
 */
public class Main {
    
    public static void main(String[] args) throws Exception {
        Buffer buffer = new Buffer();
        int numThreads = 5;
        ThreadArg[] threadArgs = new ThreadArg[numThreads];
        Thread[] thread = new Thread[numThreads];
        for(int i = 0;i<numThreads;i++){
            threadArgs[i] = new ThreadArg(buffer);
            thread[i] = new Thread(threadArgs[i]);
            thread[i].start();
        }
        for(int i = 0; i<numThreads;i++)
            thread[i].join();
        buffer.imprimir_par();
        buffer.imprimir_impar();
        //Elimina a FIFO.
        Runtime.getRuntime().exec("unlink fifo");
   }
   
 }