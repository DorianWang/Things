import java.util.ArrayList;

/**
 * Hand models a hand of cards held by a player.
 * 
 * @author Lynn Marshall and Dorian Wang
 * @version 1.1.1 May 24, 2016
 *
 */
public class Hand
{
    /** 
     * The cards are stored in a linked-list implementation of the
     * List collection.
     */
    private ArrayList<Card> cards;

    /**
     * Constructs a new, empty hand.
     */
    public Hand()
    {   
        cards = new ArrayList<Card>();
    }
    
    /**
     * Adds the specified card to this hand.
     * @param aCard the card to be added
     */
    public void addCard(Card aCard)
    {
        if (aCard != null)
            cards.add(aCard);
    }
     
   /**
     * Removes a card from this hand. Cards are removed in the order in
     * which they were added to the hand.
     * @return the card that was removed from the hand
     */
    public Card playCard()
    {
        if (cards.isEmpty()){
            return null;
        }
        Card tempCard = cards.get(0);
        cards.remove(0);
        return tempCard;
    }

    /**
     * Returns the number of cards that are currently in this hand.
     * 
     * @return the number of cards in the hand
     */    
    public int size()
    {
        return cards.size();
    }

    /**
     * Determines if this hand is empty.
     * 
     * @return true if the hand is empty, false otherwise
     */    
    public boolean isEmpty()
    {
        return cards.isEmpty();
    }

    /**
     * Returns a String that lists the ranks (but not the suits) of all the 
     * cards in this hand, starting with the first card and finishing with
     * the last card. For example, if the first card is the two of hearts, 
     * the second card is the five of diamonds, and the last card is the
     * queen of clubs, the String returned by this method will be: "2 5 12".
     * 
     * @return a String which lists the ranks of the cards in the hand
     */
    public String toString()
    {
        if (isEmpty()){ 
            return "";
        }
        String tempString = new String("");
        for (Card nextCard : cards){
            tempString = tempString + nextCard.rank() + " ";
        }
        return tempString.substring(0, tempString.length()-1);
    }
}










