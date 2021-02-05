
/**
 * Write a description of class Beamer here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */
public class Beamer extends Item
{
    // instance variables - replace the example below with your own
    private boolean charged;
    private Room location;

    /**
     * Constructor for objects of class Beamer
     */
    public Beamer()
    {
        super("Beamer", 2);
        // initialise instance variables
        charged = false;
        location = null;
    }

    /**
     * An example of a method - replace this comment with your own
     * 
     * @param currentRoom the room the beamer is to be saved to.
     * @return true if successful, false otherwise
     */
    public boolean charge(Room currentRoom)
    {
        if (charged){
            return false;
        }
        location = currentRoom;
        return true;
    }
    
    
     /**
     * An example of a method - replace this comment with your own
     * 
     * @return the saved location where the beamer was charged.
     */
    public Room fire()
    {
        if (charged){
            charged = false;
            return location;
        }
        return null;
    }
}


























