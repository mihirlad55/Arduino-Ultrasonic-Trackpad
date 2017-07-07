import com.pi4j.io.serial.Serial;
import gnu.io.*;
import org.ardulink.core.Link;
import org.ardulink.core.convenience.Links;

import java.awt.Robot;
import java.awt.AWTException;
import java.awt.event.InputEvent;
import java.io.*;
import java.util.*;

import static java.lang.Math.abs;


public class Main implements Runnable, SerialPortEventListener{

    static Robot robot;
    static Enumeration portList;
    static CommPortIdentifier portId;

    public static void main(String[] args) {
        portList = CommPortIdentifier.getPortIdentifiers();

        while (portList.hasMoreElements()) {
            portId = (CommPortIdentifier) portList.nextElement();
            if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
                if (portId.getName().equals("COM10")) {
                    Main reader = new Main();
                }
            }
        }
        try {
            robot = new Robot();
            robot.mouseMove(0,0);
        } catch(AWTException ex)
        {

        }

    }



        InputStream inputStream;
        SerialPort serialPort;
        Thread readThread;

        public Main() {
            try {
                serialPort = (SerialPort) portId.open("SimpleReadApp", 2000);
            } catch (PortInUseException e) {
                System.out.println(e);
            }
            try {
                inputStream = serialPort.getInputStream();
            } catch (IOException e) {
                System.out.println(e);
            }
            try {
                serialPort.addEventListener(this);
            } catch (TooManyListenersException e) {
                System.out.println(e);
            }
            serialPort.notifyOnDataAvailable(true);
            try {
                serialPort.setSerialPortParams(68000,
                        SerialPort.DATABITS_8,
                        SerialPort.STOPBITS_1,
                        SerialPort.PARITY_NONE);
            } catch (UnsupportedCommOperationException e) {
                System.out.println(e);
            }
            readThread = new Thread(this);
            readThread.start();
            int x = 0, y = 0, newX = 0, newY = 0;
            String sx = "", sy = "";

                    byte[] readingBuffer = new byte[5000];

                    int numBytes = 0;
                    try {
                        while (inputStream.available() > 0) {
                            numBytes = inputStream.read(readingBuffer);
                        }

                        // System.out.print(new String(readBuffer));
                        Boolean isXDone = false;

                        bufferedString = new String(readingBuffer);


                       try { robot = new Robot(); } catch (Exception ignored) { }
                        robot.mouseMove(x, y);

                        /*for (char c : bufferedString.toCharArray())
                        {
                            if (c != '\n' && c != '\r' && c != 'L' && c != 'K' && c != 'R' && c != 'E' && c != 'T' && c != '|') {
                                if (c != ':' && c != ',' && !isXDone) sx += c;
                                else if (c != ':' && c != ',' && isXDone) sy += c;
                                else if (c == ',') isXDone = true;
                                else if (c == ':') {

                                    try {
                                        System.out.println(sx + "," + sy + ":" + "\r\n");
                                        newX = (int) (Double.parseDouble(sx) * 1d);
                                        newY = (int) (Double.parseDouble(sy) * 1d);
                                        if (abs(newX - x) < 4000) x = newX;
                                        if (abs(newY - y) < 4000) y = newY;

                                        robot.mouseMove(x, y);
                                    } catch (Exception e) { System.out.println(e.getMessage());}

                                    sx = "";
                                    sy = "";
                                    isXDone = false;

                                }
                            }
                        }*/


                        // OutputStream out = serialPort.getOutputStream();
                        // out.write(("|").getBytes());
                        // out.flush();

                    } catch (IOException e) {
                        System.out.println(e);
                    }
        }

        public void run() {
            try {
                Thread.sleep(0);
            } catch (InterruptedException e) {System.out.println(e);}
        }


        String bufferedString = "";
        int pointer = 0;
        Boolean isLeftClickDown = false;
        Boolean isRightClickDown = false;

        public void serialEvent(SerialPortEvent event) {
            int x = 0, y = 0, newX = 0, newY = 0;
            String sx = "", sy = "";
           switch(event.getEventType()) {
                case SerialPortEvent.BI:
                case SerialPortEvent.OE:
                case SerialPortEvent.FE:
                case SerialPortEvent.PE:
                case SerialPortEvent.CD:
                case SerialPortEvent.CTS:
                case SerialPortEvent.DSR:
                case SerialPortEvent.RI:
                case SerialPortEvent.OUTPUT_BUFFER_EMPTY:
                    break;
                case SerialPortEvent.DATA_AVAILABLE:
                    byte[] readingBuffer = new byte[5000];

                    int numBytes = 0;
                    try {
                        while (inputStream.available() > 0) {
                            numBytes = inputStream.read(readingBuffer);
                        }

                      // System.out.print(new String(readBuffer));
                        Boolean isXDone = false;

                        bufferedString = new String(readingBuffer);

                        if (bufferedString.contains("L") && !isLeftClickDown){
                            robot.mousePress(InputEvent.BUTTON1_DOWN_MASK);
                            isLeftClickDown = true;
                        }
                        else if (bufferedString.contains("K") && isLeftClickDown) {
                            robot.mouseRelease(InputEvent.BUTTON1_DOWN_MASK);
                            isLeftClickDown = false;
                        }

                        if (bufferedString.contains("R") && !isRightClickDown) {
                            robot.mousePress(InputEvent.BUTTON3_DOWN_MASK);
                            isRightClickDown = true;
                        }
                        else if (bufferedString.contains("E") && isRightClickDown) {
                            robot.mouseRelease(InputEvent.BUTTON3_DOWN_MASK);
                            isRightClickDown = false;
                        }

                        char bufferedChars[] = bufferedString.toCharArray();
                        boolean isStartBitPresent = false;

                        for (char c : bufferedChars)
                        {
                            if (c == '|') isStartBitPresent = true;

                            if (isStartBitPresent) {
                                if (c != '\n' && c != '\r' && c != 'L' && c != 'K' && c != 'R' && c != 'E' && c != 'T' && c != '|') {
                                    if (c != ':' && c != ',' && !isXDone) sx += c;
                                    else if (c != ':' && c != ',' && isXDone) sy += c;
                                    else if (c == ',') isXDone = true;
                                    else if (c == ':') {
                                        System.out.println(sx + "," + sy + ":" + "\r\n");
                                        newX = (int) (Double.parseDouble(sx) * 1d);
                                        newY = (int) (Double.parseDouble(sy) * 1d);
                                        if (abs(newX - x) < 4000) x = newX;
                                        if (abs(newY - y) < 4000) y = newY;


                                        isStartBitPresent = false;
                                        sx = "";
                                        sy = "";
                                        isXDone = false;
                                        robot.mouseMove(x, y);

                                    }
                                }
                            }
                        }

                       // OutputStream out = serialPort.getOutputStream();
                       // out.write(("|").getBytes());
                       // out.flush();

                    } catch (IOException e) {System.out.println(e);}
                    break;
            }
        }
    }


