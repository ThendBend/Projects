package com.training.main;

import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

public class MouseClick extends MouseAdapter {

	public void mouseClicked(MouseEvent e) {
		System.out.println(e.getX() + ", " + e.getY());
		if(e.getX() > 260 && e.getX() < 260 + 80 && e.getY() > 120 && e.getY() < 120 + 20 && HUD.keyEsc == 1) {
			HUD.mpressInv *= -1;
			HUD.keyEsc *= -1;
		}else if(e.getX() > 260 && e.getX() < 260 + 80 && e.getY() > 150 && e.getY() < 150 + 20 && HUD.keyEsc == 1) {
			HUD.mpressExit *= -1;
			HUD.keyEsc *= -1; 
		}else if(e.getX() > 200 && e.getX() < 200 + 80 && e.getY() > 190 && e.getY() < 190 + 20 && HUD.mpressExit == 1) {
			System.exit(0);
		}else if(e.getX() > 310 && e.getX() < 310 + 80 && e.getY() > 190 && e.getY() < 190 + 20 && HUD.mpressExit == 1) {
			HUD.mpressExit *= -1;
			HUD.keyEsc *= -1;
		}
	}
	
}
