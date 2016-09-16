import java.util.*;

/**
 * A Card is a playing card from an Anglo-American deck of cards.
 * 
 * @author Lynn Marshall and Dorian Wang
 * @version 1.1.1 May 24, 2016
 */


public class Card
{
    /** The card's suit: "hearts", "diamonds", "clubs", "spades". */
    private String suit;
    
    private static final Set <String> allSuits =
                     new HashSet<String>(Arrays.asList(
                     new String[]{"hearts", "diamonds", "clubs", "spades"}));
    
    /** 
     * The card's rank, between 1 and 13 (1 represents the ace, 11 represents
     * the jack, 12 represents the queen, and 13 represents the king.)
     */
    private int rank;

    /**
     * Constructs a new card with the specified suit and rank.
     * Creates an ace of hearts if the input is incorrect.
     * 
     * @param suit a String representing the card's suit
     * @param rank the rank of the card, from 1 to 13, for ace to king
     * 
     */
    public Card(String suit, int rank)
    {
       if (allSuits.contains(suit)){
           this.suit = suit;
       }
       else
       {
           this.suit = "hearts";
       }
       
       if (rank <= 13 && rank >= 1){
          this.rank = rank;
       }
       else
       {
           this.rank = 1;
       }
    }
    
    /**
     * Returns this card's suit.
     * @return the string representing the card's suit
     */
    public String suit()
    {
        return suit;
    }
    
    /**
     * Returns this card's rank.
     * @return the card's rank
     */
    public int rank()
    {
        return rank;
    }
    
    /**
     * Determines if this card has the same rank as the specified card.
     * @param aCard the card to be compared
     * @return true if the card has the same rank, false otherwise
     */
    public boolean hasSameRank(Card aCard)
    {
        if (aCard != null){
            return (aCard.rank() == rank());
        }
        return false;
    }
    
    /**
     * Determines if this card is equal to the specified card.
     * @param aCard the card to be compared
     * @return true if the card has the same rank and suit, false otherwise
     */
    public boolean isEqualTo(Card aCard)
    {
        if (aCard != null){
            return (aCard.rank() == rank() && aCard.suit().equals(suit()));
        }
        return false;
    }
}











