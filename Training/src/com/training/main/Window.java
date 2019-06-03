package com.training.main;

import java.awt.Dimension;
import java.awt.Toolkit;

import javax.swing.JFrame;

public class Window extends JFrame{

	private static final long serialVersionUID = 4633598964616527721L;
	
	public Window(int width, int height, String title, Game game) {
		JFrame frame = new JFrame(title);
		frame.setIconImage(Toolkit.getDefaultToolkit().getImage(getClass().getResource("/swordico.png")));
		
		frame.setPreferredSize(new Dimension(width, height));
		frame.setMaximumSize(new Dimension(width, height));
		frame.setMinimumSize(new Dimension(width, height));
		
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setResizable(false);
		frame.setLocationRelativeTo(null);
		frame.add(game);
		frame.setVisible(true);
		game.start();
		
	}
}
