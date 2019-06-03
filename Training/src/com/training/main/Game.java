package com.training.main;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.image.BufferStrategy;
import java.awt.image.BufferedImage;
import java.util.Random;


public class Game extends Canvas implements Runnable{

	private static final long serialVersionUID = 8362940575581470823L;
	
	public static final int WIDTH = 640, HEIGHT = WIDTH / 12 * 9;
	
	public static int storyMode = 0;
	public static boolean storyRun = false;
	public static boolean travel = false;
	public static boolean fight = false;
	private int cnt = 0;
	private int blue = 255;
	private int red = 0;
	
	private Thread thread;
	private boolean running = false;
	private boolean rain = false;
	
	String allMonsters[] = {"Public Toilet","Desert","House Party","Christmas","Work Office"};
	
	private Handler handler;
	private Random r;
	private HUD hud;
	
	private BufferedImage sky;
	private BufferedImage ground;
	
	public Game() {
		new Window(WIDTH, HEIGHT, "The Heavens", this);
		
		handler = new Handler();
		hud = new HUD();
		r = new Random();
		
		sky = ImageLoader.loadImage("/sky.png");
		ground = ImageLoader.loadImage("/ground.png");

		this.addMouseListener(new MouseClick());
		this.addKeyListener(new KeyInput(handler));
		
		handler.addObject(new Player(WIDTH/2-50, HEIGHT-200, ID.Player));	
	}
	
	public synchronized void start() {
		thread = new Thread(this);
		thread.start();
		running = true;
	}
	
	public synchronized void stop() {
		try {
			thread.join();
			running = false;
			
		}catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	public void run() {
		this.requestFocus();
		long lastTime = System.nanoTime();
		double amountOfTicks = 60.0;
		double ns = 1000000000 / amountOfTicks;
		double delta = 0;
		long timer = System.currentTimeMillis();
		int frames = 0;
		
		while(running) {
			long now = System.nanoTime();
			delta += (now - lastTime) / ns;
			lastTime = now;
			while(delta >= 1) {
				tick();
				delta--;
			}
			
			if(running)
				render();
			frames++;
			
			if(System.currentTimeMillis() - timer > 1000) {
				timer += 1000;
				System.out.println("FPS: " + frames);
				frames = 0;
			}
		}
		stop();
	}
	
	private void tick() {
		handler.tick();
		hud.tick();
		if(!travel) {
			if(r.nextInt(10000) == 1 && !rain && !travel) {
				for (int i = 0; i < 500; i++) {
					handler.addObject(new Rain(r.nextInt(WIDTH), -r.nextInt(300), ID.Rain));
					rain = true;
				}
			}
			if(r.nextInt(10000) == 1 && rain) {
				for (int i = 500; i > 0; i--) {
					GameObject tempObject = handler.object.get(i);
					handler.removeObject(tempObject);
					rain = false;
				}
			}
		}
	}
	
	private void render() {
		BufferStrategy bs = this.getBufferStrategy();
		if(bs == null) {
			this.createBufferStrategy(3);
			return;
		}
		
		Graphics g = bs.getDrawGraphics();
		
		if(!travel) {
			g.fillRect(0, 0, WIDTH, HEIGHT);
			g.drawImage(sky, 0, 0, WIDTH, HEIGHT, null);
			
			for(int x = 0; x <= WIDTH; x+=32) {
				for(int y = HEIGHT-200; y <= HEIGHT; y+=32) {
					g.drawImage(ground, x, y, null);
				}
			}
			
			g.setColor(new Color(23, 38, 6));
			g.drawLine(30, 280, 30, 300);
			g.drawLine(50, 280, 50, 300);
			g.fillRect(22, 265, 35, 20);
			g.drawLine(570, 280, 570, 300);
			g.drawLine(590, 280, 590, 300);
			g.fillRect(565, 265, 30, 20);
			g.setColor(Color.white);
			g.setFont(new Font("Arial", Font.BOLD, 9));
			g.drawString("Travel", 26, 278);
			g.drawString("Shop", 568, 278);
			
			for(int i = 0; i < handler.object.size(); i++) {
				GameObject tempObject = handler.object.get(i);
				int cnt = 0;
				g.setColor(Color.black);
				g.setFont(new Font("Arial", Font.BOLD, 14));
				if(tempObject.getId() == ID.Player && tempObject.getX() < 80) {
					for(int z = 0; z < allMonsters.length; z++) {
						if(HUD.myLevel >= ZeroOne(z, 5)) {
							g.drawString(z + 1 + " . " + allMonsters[z], 20, ((HEIGHT-200) - 150) + cnt);
						} else {
							g.drawString("Level " + z*5 + " required", 20, ((HEIGHT-200) - 150) + cnt);
						}
						cnt += 25;
					}
				}
			}
		}
		
		g.setColor(new Color(red, 0, blue));
		g.fillRect(200, 200, 100, 100);
		
		handler.render(g);
		
		hud.render(g);
	
		g.dispose();
		bs.show();
	}
	
	public static int clamp(int var, int min, int max) {
		if(var >= max) {
			return var = max;
		}else if(var <= min) {
			return var = min;
		}else {
			return var;
		}
	}
	
	public static int random(int min, int max) {
		Random r;
		r = new Random();
		while(true) {
			int i = r.nextInt(1000);
			if(i > min && i < max) {
				return i;
			}
		}
	}
	
	public static int ZeroOne(int i, int z) {
		return (i==0) ? 1 : i*z;
	}
	
	public static void main(String[] args) {
		new Game();
	}

}
