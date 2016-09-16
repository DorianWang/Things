
/**
 * Write a description of class Item here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */
public class Item
{
    // instance variables - replace the example below with your own
    private String name;
    private int weight;

    /**
     * Constructor for objects of class Item
     */
    public Item(String name, int weight)
    {
        this.name = name;
        this.weight = weight;
    }

    /**
     * An example of a method - replace this comment with your own
     * 
     * @param  y   a sample parameter for a method
     * @return     the sum of x and y 
     */
    public String getItemDescription()
    {
        return name + " - " + weight + "kg";
    }
    
    public String getName()
    {
        return name;
    }
    
}






