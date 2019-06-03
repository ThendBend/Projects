package com.training.main;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;

public class HUD {
	
	public static int expNext, myDefense, myDamage;
	
	public static int myLevel = 1;
	public static int myGold = 0;
	public static int maxHealth = 100;
	public static int myHealth = maxHealth;
	public static int myExp = 0;
	public static int monsHealth = 80;
	public static int monsDamage = 15;
	public static String[] allItems = {"Lusty Sword", "Lusty Chest", "Lusty Gloves"};
	public static int keyEsc = -1;
	public static int mpressExit = -1;
	public static int mpressInv = -1;
	public static int invOpen = -1; 
	
	public void tick() {      
		expNext = 500 * myLevel;
		myDefense = 1 * myLevel;
		myDamage = 30 * myLevel;
		maxHealth = 100 * myLevel;
	}
	
	public void render(Graphics g) {
		if(keyEsc == 1) {
			g.setColor(Color.darkGray);
			g.fillRect(235, 100, 130, 180);
			g.setColor(Color.white);
			g.fillRect(260, 120, 80, 20);
			g.fillRect(260, 150, 80, 20);
			g.setColor(Color.black);
			g.setFont(new Font("Arial", Font.BOLD, 14));
			g.drawString("Inventory", 267, 134);
			g.drawString("Exit Game", 265, 165);
			g.setColor(Color.yellow);
			g.drawString("Level : " + myLevel, 268, 200);
			g.drawString("Gold : " + myGold, 243, 250);
			g.drawString("Exp : " + myExp + " / " + expNext, 243, 270);
		}
		if(mpressExit == 1) {
			g.setColor(Color.darkGray);
			g.fillRect(170, 150, 250, 80);
			g.setColor(Color.white);
			g.fillRect(200, 190, 80, 20);
			g.fillRect(310, 190, 80, 20);
			g.setFont(new Font("Arial", Font.BOLD, 14));
			g.drawString("Are you sure you want to quit?", 190, 170);
			g.setColor(Color.black);
			g.drawString("Yes", 225, 205);
			g.drawString("No", 340, 205);
		}
	}

}
