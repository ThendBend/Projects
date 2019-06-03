package com.training.main;

import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

public class KeyInput extends KeyAdapter {
	
	private Handler handler;
	
	Cockroach cockroach = new Cockroach(260, 230, ID.Cockroach);
	
	
	public KeyInput(Handler handler) {
		this.handler = handler;
	}
	
	public void keyPressed(KeyEvent e) {
		int key = e.getKeyCode();
		for(int i = 0; i < handler.object.size(); i++) {
			GameObject tempObject = handler.object.get(i);
			if(tempObject.getId() == ID.Player) {
				if(key == KeyEvent.VK_A) {
					tempObject.setX(tempObject.getX() - 4);
				}
				if(key == KeyEvent.VK_D) {
					tempObject.setX(tempObject.getX() + 4);
					 
				}
				if(key == KeyEvent.VK_W && tempObject.canJump) {
					tempObject.setVelY(-10);
					tempObject.canJump = false;
				}
				if(key == KeyEvent.VK_ESCAPE) {
					HUD.keyEsc *= -1;
					HUD.mpressExit = -1;
					HUD.mpressInv = -1;
				}
				if(key == KeyEvent.VK_1 && tempObject.getX() < 80 && !Game.travel) {
					handler.addObject(cockroach);
					Game.storyRun = true;
					Game.travel = true;
				} else if(key == KeyEvent.VK_1 && !Game.fight && Game.storyMode == 6) {
					Cockroach.playerVelx = -4;
					Game.fight = true;
				}
				if (key == KeyEvent.VK_2 && Game.storyMode == 6 && !Game.fight) {
					HUD.myHealth = HUD.maxHealth;
					HUD.monsHealth = 80;
					cockroach.setX(260);
					cockroach.setY(230);
					cockroach.setVelX(0);
					cockroach.setVelY(0);
					Cockroach.playerVely = 0;
					Cockroach.playerVelx = 0;
					Cockroach.playerX = 600;
					Cockroach.playerY = 360;
					handler.removeObject(cockroach);
					Game.storyMode = 0;
					Game.travel = false;
					Game.fight = false;
					Cockroach.monsTurn = false;
				}
				if(key == KeyEvent.VK_ENTER && Game.storyRun) {
					Game.storyMode++;
					Game.storyRun = false;
				} else if(key == KeyEvent.VK_ENTER && Game.storyMode == 9) {
					HUD.myHealth = HUD.maxHealth;
					HUD.monsHealth = 80;
					cockroach.setX(260);
					cockroach.setY(230);
					cockroach.setVelX(0);
					cockroach.setVelY(0);
					Cockroach.playerVely = 0;
					Cockroach.playerVelx = 0;
					Cockroach.playerX = 600;
					Cockroach.playerY = 360;
					handler.removeObject(cockroach);
					Game.storyMode = 0;
					Game.travel = false;
					Game.fight = false;
					Cockroach.monsTurn = false;
				}
			}
		}
	}
	public void keyReleased(KeyEvent e) {
	}
}
