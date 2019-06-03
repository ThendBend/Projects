package com.training.main;

import java.awt.Color;
import java.awt.Graphics;
import java.util.Random;

public class Rain extends GameObject{

	private Random r;
	
	public Rain(int x, int y, ID id) {
		super(x, y, id);
		r = new Random();
		velY = r.nextInt(7) + 3;
	}

	public void tick() {
		y += velY;
		if (y > Game.HEIGHT) {
			y = -r.nextInt(300);
		}
	}

	public void render(Graphics g) {
		g.setColor(Color.cyan);
		g.fillRect(x, y, 3, 6);
		
	}

}
