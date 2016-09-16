import java.util.Random;
import java.util.ArrayList;

/**
 * Write a description of class TransporterRoom here.
 * 
 * @author Dorian Wang
 * @version Today's version, 1.0
 */
public class TransporterRoom extends Room
{
    
     /**
     * Constructor.
     *
     */
    public TransporterRoom(String description)
    {
        super(description);
    }
    
    
    /**
     * Returns a random room, independent of the direction parameter.
     *
     * @param direction Ignored.
     * @return A randomly selected room.
     */
    public Room getExit(String direction)
    {
        Room tempRoom = this;
        while(tempRoom == this){
            tempRoom = findRandomRoom();
        }
            
        //Never teleports back.
        return tempRoom;
    }

    /**
     * Choose a random room.
     *
     * @return The room we end up in upon leaving this one.
     */
    private Room findRandomRoom()
    {
        //Debugging is for the next poor souless coder.
        return allTheRooms.get((new Random()).nextInt(allTheRooms.size()));
    }
}












