package main;

import java.util.concurrent.atomic.AtomicBoolean;

public class Chef implements Runnable {
	
	private Thread worker;
	private Ingredient stock;
	private ServingTable table;
	private final AtomicBoolean running = new AtomicBoolean(false);
	
	Chef(Ingredient newstock, ServingTable table){
		this.stock = newstock;
		this.table = table;
	}
	
	public void start() {
		worker = new Thread(this);
        worker.start();
	}
	
	public void run() {
		this.running.set(true);
		while(this.running.get()) {
			table.get(this.stock); // Get ingredients from table
			System.out.println("Making League Player with chef " + stock);
			table.addLeaguePlayer(); // If ingredients were gotten, add one to the counter
		}
	}
	
	public void stop() {
		System.out.println("Stopping Chef!");
		this.running.set(false);
	}
	
	
}
