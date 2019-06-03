package com.training.main;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.util.Random;

public class Cockroach extends GameObject {
	
	private BufferedImage publicToilet, textBox, textBox2, character, toilet, cockroach, stain;
	
	private int sizeX = 32;
	private int sizeY = 32;
	
	private Random r;
	
	public static boolean monsTurn = false;
	
	public static int playerX = 600;
	public static int playerY = 360;
	public static int playerVelx, playerVely;
	
	private int gold, exp;
	
	
	public Cockroach(int x, int y, ID id) {
		super(x, y, id);
		publicToilet = ImageLoader.loadImage("/publictoilet.png");
		textBox = ImageLoader.loadImage("/textbox.png");
		textBox2 = ImageLoader.loadImage("/textbox2.png");
		character = ImageLoader.loadImage("/char.png");
		toilet = ImageLoader.loadImage("/toilet.png");
		cockroach = ImageLoader.loadImage("/cockroach.png");
		stain = ImageLoader.loadImage("/stain.png");
		
		r = new Random();
	}

	public void tick() {
		playerX +=  playerVelx;
		playerY += playerVely;
		x += velX;
		y += velY;
		if(Game.storyMode == 1)
		{
			playerVelx = -4;
			sizeX += 1;
			sizeY += 1;
		}
		if(playerX < 400 && Game.storyMode == 1) {
			playerX = 500;
			playerY = 250;
			Game.storyMode++;
			playerVelx = 0;
			Game.storyRun = true;
		}
		if(Game.storyMode == 3 && playerX >= 400) {
			playerVelx = -3;
			playerVely = -1;
			sizeX = 32;
			sizeY = 32;
		}
		if(playerX < 400 && Game.storyMode == 3) {
			playerVelx = 0;
			playerVely = 0;
			Game.storyRun = true;
		}
		if(Game.storyMode == 4) {
			Game.storyRun = true;
		}
		if(Game.storyMode == 5 && playerX <= 500) {
			velX = 2;
			velY = 1;
			playerVelx = 2;
			playerVely = 1;
		}
		if(Game.storyMode == 5 && playerX > 500) {
			velX = 0;
			velY = 0;
			playerVelx = 0;
			playerVely = 0;
			Game.storyMode++;
			System.out.println(Game.storyMode);
		}
		if(x < 363 && Game.storyMode == 6 && monsTurn && Game.fight) {
			velX = 0;
			Game.fight = false;
			monsTurn = false;
			HUD.myHealth -= r.nextInt(HUD.monsDamage);
		}
		if(x > 460 && monsTurn && Game.fight && Game.storyMode == 6) {
			velX = -4;
		}
		if(playerX > 505 && !monsTurn && Game.fight && Game.storyMode == 6) {
			HUD.monsHealth -= r.nextInt(500);
			playerVelx = 0;
			velX = 4;
			monsTurn = true;
		}
		if(playerX < 420 && !monsTurn && Game.fight && Game.storyMode == 6) {
			playerVelx = 4;
		}
		if(Game.storyMode == 6 && HUD.monsHealth < 0 ) {
			exp = Game.random(100, 200);
			gold = Game.random(50, 100);
			HUD.myExp += exp;
			HUD.myGold += gold;
			
			Game.storyMode++;
			Game.storyRun = true;
		}
		if(Game.storyMode == 6 && HUD.myHealth < 0 ) {
			Game.storyMode += 2;
			Game.storyRun = true;
		}
		if(Game.storyMode == 7) {
			Game.storyRun = true;
		}
		if(Game.storyMode == 8) {
			Game.storyRun = true;
		}
	}

	public void render(Graphics g) {
		g.setFont(new Font("Arial", Font.BOLD, 14));
		if (Game.storyMode == 0 || Game.storyMode == 1 && Game.travel) {
			g.drawImage(publicToilet, 0, 0, 630, 440, null);
			g.setColor(Color.black);
			g.setFont(new Font("Arial", Font.BOLD, 14));
			if(Game.storyMode == 0) {
				g.drawImage(textBox2, 430, 260, null);
				g.drawString("Ah i need", 480, 300);
				g.drawString("to piss...", 480, 320);
		}
			g.drawImage(character, playerX, playerY, sizeX, sizeY, null);
		}
		if(Game.storyMode == 2 || Game.storyMode == 3 || Game.storyMode == 4 || Game.storyMode == 5 || Game.storyMode == 6 || Game.storyMode == 7 || Game.storyMode == 8 || Game.storyMode == 9) {
			g.drawImage(toilet, 0, 0, 630, 440, null);
			if(Game.storyMode == 2) {
				g.drawImage(textBox2, 300, 120, 250, 200, null);
				g.drawString("Woah! Why is", 353, 165);
				g.drawString("there a hole", 353, 185);
				g.drawString("near the", 356, 205);
				g.drawString("washbowl?", 360, 225);
			}
			if (playerX < 400 && Game.storyMode == 3) {
				g.drawImage(textBox, 340, 80, null);
				g.drawString("Hmm...", 403, 120);
			}
			if (Game.storyMode == 4 || Game.storyMode == 5 || Game.storyMode == 6 ) {
				if (Game.storyMode == 4) {
					g.drawImage(textBox, 320, 30, 250, 200, null);
					g.drawString("Holymolly! That's", 372, 100);
					g.drawString("one big ass", 372, 120);
					g.drawString("cockroach!", 372, 140);
				}
				if (Game.storyMode == 6 && !Game.fight) {
					g.drawString("1 . Attack", 500, 200);
					g.drawString("2 . Run away", 500, 220);
				}
				if(Game.storyMode == 6) {
					g.setColor(Color.yellow);
					g.drawString("Health : " + HUD.myHealth, playerX, playerY-10);
					g.drawString("Health : " + HUD.monsHealth, x, y-10);
				}
				g.drawImage(cockroach, x, y, null);
			}
			if(Game.storyMode == 7 || Game.storyMode == 9) {
				g.drawImage(stain, 350, 290, null);
				g.drawImage(textBox, 445, 130, null);
				if(Game.storyMode == 7 && HUD.myHealth > 0) {
					g.setColor(Color.black);
					g.drawString("Damn, That", 490, 180);
					g.drawString("was close!", 490, 200);
				}
				if(HUD.myHealth < 0 && Game.storyMode == 7) {
					g.drawString("I'm hurt", 510, 180);
					g.drawString("Badly!", 510, 200);
				}
				if(Game.storyMode == 9) {
				g.drawString("I need to", 500, 175);
				g.drawString("go back", 500, 195);
				g.drawString("now.", 500, 215);
				}			
			}
			if(Game.storyMode == 8) {
				g.drawImage(stain, 350, 290, null);
				g.drawImage(textBox2, 330, 130, 250, 180, null);
				g.drawString("I found " + gold, 390, 165);
				g.drawString("gold and", 390, 185);
				g.drawString("became", 395, 205);
				g.drawString("stronger", 395, 225);
				g.drawString("by gaining ", 400, 245);
				g.drawString(exp + " exp", 410, 265);
			}
			g.drawImage(character, playerX, playerY, null);
		}
	}
}
