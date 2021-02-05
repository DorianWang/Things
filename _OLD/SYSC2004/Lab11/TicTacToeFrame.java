import java.util.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

/**
 * A class modelling a tic-tac-toe (noughts and crosses, Xs and Os) game in a very
 * simple GUI window.
 * 
 * @author Lynn Marshall and Dorian Wang
 * @version June 7, 2016
 */

public class TicTacToeFrame extends TicTacToe 
{ 
   private JTextArea status; // text area to print game status
   private JScrollPane newPane;
   private JFrame frame;
   
   /** 
    * Constructs a new Tic-Tac-Toe board and sets up the basic
    * JFrame containing a JTextArea in a JScrollPane GUI.
    */
   public TicTacToeFrame()
   { 
       super();
       status = new JTextArea(12, 14);
       newPane = new JScrollPane(status);
       frame = new JFrame("Tic tac toe");
       
       status.setFont(new Font("monospaced", Font.PLAIN, 12));
       
       frame.setSize(100, 200);
       
       frame.add(newPane);
       frame.setVisible(true);
       
       playGame();

   }
   
   /**
    * Prints the board to the GUI using toString().
    */
    public void print() 
    {  
        status.setText(toString());
    }

}