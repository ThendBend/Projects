package com.training.main;

import java.awt.Graphics;
import java.awt.image.BufferedImage;

public class Player extends GameObject {
	
	private BufferedImage image;
	
	public Player(int x, int y, ID id) {
		super(x, y, id);
		image = ImageLoader.loadImage("/char.png");
		velY = 4;
	}

	public void tick() {
		x += velX;
		y += velY;
		if (y == Game.HEIGHT - 200) {
			canJump = true;
		}
		if (y < 200) {
			velY = 4;
		}
		if(HUD.myExp >= HUD.expNext) {
			HUD.myLevel++;
			HUD.myExp = HUD.myExp - HUD.expNext;
		}
		x = Game.clamp(x, 0-16, Game.WIDTH-62);
		y = Game.clamp(y, 0, Game.HEIGHT-200);
	}

	public void render(Graphics g) {
		g.drawImage(image, x, y, null);
	}
}
