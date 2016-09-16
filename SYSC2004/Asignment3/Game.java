import java.util.Stack;
/**
 *  This class is the main class of the "World of Zuul" application. 
 *  "World of Zuul" is a very simple, text based adventure game.  Users 
 *  can walk around some scenery. That's all. It should really be extended 
 *  to make it more interesting!
 * 
 *  To play this game, create an instance of this class and call the "play"
 *  method.
 * 
 *  This main class creates and initialises all the others: it creates all
 *  rooms, creates the parser and starts the game.  It also evaluates and
 *  executes the commands that the parser returns.
 * 
 * @author  Michael Kolling and David J. Barnes
 * @version 2006.03.30
 * 
 * @author Lynn Marshall
 * @version October 21, 2012
 * 
 * @author  Dorian Wang
 * @version June 2nd, 2016
 */

public class Game 
{
    private Parser parser;
    private Room currentRoom;
    private Room lastRoom;
    private Stack <Room> roomStack;
    private Item heldItem;
    private int numPickupsLeft;
    
    private static final int PICKUPS_PER_FOOD = 5;
    private static final String NAME_FOOD = "blood";
    
        
    /**
     * Create the game and initialise its internal map.
     */
    public Game() 
    {
        createRooms();
        parser = new Parser();
        lastRoom = null;
        roomStack = new Stack <Room>();
        heldItem = null;
        numPickupsLeft = 0;
    }

    /**
     * Create all the rooms and link their exits together.
     */
    private void createRooms()
    {
        Room outside, theatre, pub, lab, office, transporter;
      
        // create the rooms
        outside = new Room("outside the main entrance of the university");
        theatre = new Room("in a lecture theatre");
        pub = new Room("in the campus pub");
        lab = new Room("in a computing lab");
        office = new Room("in the computing admin office");
        transporter = new TransporterRoom("in a transporting device");
        
        // initialise room exits
        outside.setExit("east", theatre);
        outside.setExit("south", lab);
        outside.setExit("west", pub);
        outside.setExit("north", transporter);

        theatre.setExit("west", outside);

        pub.setExit("east", outside);

        lab.setExit("north", outside);
        lab.setExit("east", office);

        office.setExit("west", lab);
        
        Item testItem = new Item("Fir", 123);
        
        Item foodItem = new Item(NAME_FOOD, 1);
        outside.addItem(testItem);
        outside.addItem(foodItem);
        outside.addItem(foodItem);
        
        Beamer testBeamer = new Beamer();
        outside.addItem(testBeamer);
        theatre.addItem(testBeamer);
        theatre.addItem(foodItem);
        
        currentRoom = outside;  // start game outside
    }

    /**
     *  Main play routine.  Loops until end of play.
     */
    public void play() 
    {            
        printWelcome();

        // Enter the main command loop.  Here we repeatedly read commands and
        // execute them until the game is over.
                
        boolean finished = false;
        while (! finished) {
            Command command = parser.getCommand();
            finished = processCommand(command);
        }
        System.out.println("Thank you for playing.  Good bye.");
    }

    /**
     * Print out the opening message for the player.
     */
    private void printWelcome()
    {
        System.out.println();
        System.out.println("Welcome to the World of Zuul!");
        System.out.println("World of Zuul is a new, incredibly boring adventure game.");
        System.out.println("Type 'help' if you need help.");
        System.out.println();
        System.out.println(currentRoom.getLongDescription());
    }

    /**
     * Given a command, process (that is: execute) the command.
     * 
     * @param command The command to be processed
     * @return true If the command ends the game, false otherwise
     */
    private boolean processCommand(Command command) 
    {
        boolean wantToQuit = false;

        if(command.isUnknown()) {
            System.out.println("I don't know what you mean...");
            return false;
        }

        String commandWord = command.getCommandWord();
        if (commandWord.equals("help")) {
            printHelp();
        }
        else if (commandWord.equals("go")) {
            goRoom(command);
        }
        else if (commandWord.equals("quit")) {
            wantToQuit = quit(command);
        }
        else if (commandWord.equals("look")){
            look(command);
        }
        else if (commandWord.equals("eat")){
            eat(command);
        }
        else if (commandWord.equals("back")){
            back(command);
        }
        else if (commandWord.equals("stackBack")){
            stackBack(command);
        }
        else if (commandWord.equals("take")){
            take(command);
        }
        else if (commandWord.equals("drop")){
            drop(command);
        }
        else if (commandWord.equals("fire")){
            fire(command);
        }
        else if (commandWord.equals("charge")){
            charge(command);
        }
        
        // else command not recognised.
        return wantToQuit;
    }

    // implementations of user commands:

    /**
     * Print out some help information.
     * Here we print a cryptic message and a list of the 
     * command words.
     */
    private void printHelp() 
    {
        System.out.println("You are lost. You are alone. You wander");
        System.out.println("around at the university.");
        System.out.println();
        System.out.println("Your command words are:");
        System.out.println(parser.getCommands());
    }

    /** 
     * Print the description of the room, and the carried item.
     */
    private void printDescription()
    {
        System.out.println("You care carrying: " + heldItem.getName());
        System.out.println(currentRoom.getLongDescription());
    }
    
    
    /** 
     * Try to go to one direction. If there is an exit, enter the new
     * room, otherwise print an error message.
     * 
     * @param command The command to be processed
     */
    private void goRoom(Command command) 
    {
        if(!command.hasSecondWord()) {
            // if there is no second word, we don't know where to go...
            System.out.println("Go where?");
            return;
        }

        String direction = command.getSecondWord();

        // Try to leave current room.
        Room nextRoom = currentRoom.getExit(direction);

        if (nextRoom == null) {
            System.out.println("There is no door!");
        }
        else {
            lastRoom = currentRoom;
            roomStack.add(currentRoom);
            currentRoom = nextRoom;
            System.out.println(currentRoom.getLongDescription());
        }
    }

    /** 
     * "Quit" was entered. Check the rest of the command to see
     * whether we really quit the game.
     * 
     * @param command The command to be processed
     * @return true, if this command quits the game, false otherwise
     */
    private boolean quit(Command command) 
    {
        if(command.hasSecondWord()) {
            System.out.println("Quit what?");
            return false;
        }
        else {
            return true;  // signal that we want to quit
        }
    }
    
    
     /** 
     * "Look" was entered. 
     * 
     * Prints the description of the room.
     */
    private void look(Command command) 
    {
        if(command.hasSecondWord()) {
            System.out.println("Look what?");
        }
        else{
            printDescription();
        }
    }
    
    
     /** 
     * "Back" was entered. 
     * 
     * Returns the player to their previous location.
     */
    private void back(Command command) 
    {
        if(command.hasSecondWord()) {
            System.out.println("Back what?");
        }
        else if (lastRoom == null){
            System.out.println("Unable to go back.");
        }
        else {
            Room tempRoom = currentRoom;
            roomStack.add(currentRoom);
            currentRoom = lastRoom;
            lastRoom = tempRoom;
            printDescription();
        }
        
    }
    
     /** 
     * "stackBack" was entered. 
     * 
     * Returns the player to their previous location.
     * Records the previous locations the player has traveled to, and returns there.
     */
    private void stackBack(Command command) 
    {
        //Do nothing yet
        if(command.hasSecondWord()) {
            System.out.println("Back what?");
        }
        else if (roomStack.empty() == true){
            System.out.println("Unable to go back.");
        }
        else
        {
            lastRoom = currentRoom;
            currentRoom = roomStack.pop();
            printDescription();
        }
        
    }
    
     /** 
     * "take" was entered. 
     * 
     * Takes the item specified.
     */
    private void take(Command command) 
    {
        //Do nothing yet
        if(command.hasSecondWord() == false) {
            System.out.println("Take what?");
        }
        else if (heldItem != null){
            System.out.println("You are already holding something.");
        }
        else
        {
            Item tempItem = currentRoom.getItem(command.getSecondWord());
            if (tempItem == null){
                System.out.println("That item is not in the room.");
            }
            else
            {
                if (numPickupsLeft > 0 || command.getSecondWord().equals(NAME_FOOD)){
                    heldItem = tempItem;
                    System.out.println("You picked up: " + heldItem.getName());
                }
                else
                {
                    System.out.println("You are hungry and must eat first.");
                }
            }
        }
        
    }
    
     /** 
     * "drop" was entered. 
     * 
     * Drops the held item, and places it in the room.
     */
    private void drop(Command command) 
    {
        //Do nothing yet
        if(command.hasSecondWord() == false) {
            System.out.println("Drop what?");
        }
        else if (heldItem == null){
            System.out.println("You are not holding anything.");
        }
        else
        {
            currentRoom.addItem(heldItem);
            System.out.println("You have dropped " + heldItem.getName());
            heldItem = null;
        }
        
    }
    
    /** 
     * "eat" was entered. 
     * 
     * Eats the held item if it is a "glob of blood"
     */
    private void eat(Command command) 
    {
        //Do nothing yet
        if(command.hasSecondWord() == false) {
            System.out.println("Eat what?");
        }
        else if (heldItem == null){
            System.out.println("You are not holding anything.");
        }
        else if(heldItem.getName().equals(NAME_FOOD))
        {
            if (numPickupsLeft != 0){
                System.out.println("You are not hungry yet");
            }
            else{
                System.out.println("You have eatten " + heldItem.getName());
                heldItem = null;
                numPickupsLeft = PICKUPS_PER_FOOD;
            }
        }
        else
        {
            System.out.println("You have no food to eat");
        }
        
    }
    
     /** 
     * "charge" was entered. 
     * 
     * Charges the beamer.
     */
    private void charge(Command command) 
    {
        //Do nothing yet
        if(command.hasSecondWord() == false) {
            System.out.println("Charge what?");
        }
        else if (heldItem instanceof Beamer){
            boolean tempCharge = ((Beamer) heldItem).charge(currentRoom);
            if (tempCharge == false){
                System.out.println("Beamer is already charged");
            }
            else
            {
                System.out.println("Zhmmm!");
            }
        }
        else
        {
            System.out.println("You are not holding a beamer.");
        }
        
    }
    
     /** 
     * "fire" was entered. 
     * 
     * Fires the beamer, returning to the charged location.
     */
    private void fire(Command command) 
    {
        //Do nothing yet
        if(command.hasSecondWord() == false) {
            System.out.println("Fire what?");
        }
        else if (heldItem instanceof Beamer){
            Room tempRoom = ((Beamer) heldItem).fire();
            if (tempRoom == null){
                System.out.println("Beamer is not charged");
            }
            else
            {
                System.out.println("Pew!");
                roomStack.add(currentRoom);
                lastRoom = currentRoom;
                currentRoom = tempRoom;
                printDescription();
            }
        }
        else
        {
            System.out.println("You are not holding a beamer.");
        }
        
    }
    
}






















