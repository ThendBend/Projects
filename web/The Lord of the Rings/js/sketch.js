var data;
var str;
var pos;
var a;
var cnt = 0;
var ans = "";
var ifAns;
var senten;
var inputText;
var output;
var loading = true;

function fileLoaded(file){
	noCanvas();
	senten = file + '';
	str = split(senten, '.');
	select("#paradive").html("Press Enter to dive deeper into Middle Earth.");
	select("#paratype").html("Type something you like about The Lord of the Rings:");
	inputText = createInput();
	inputText.elt.focus();
	inputText.input(function lel(){
		a = 0;
		pos = 0;
		cnt = 0;
		toreply();
	});
	output = select("#para");
	loading = false;
}

function progress(percent) {
	console.log(percent);
	console.log("hey");
}

function error(err) {
	console.log(err);
}

function setup() {
	loadStrings('tlotrwow123.txt', fileLoaded, progress, error);
}

function draw() {
	if(loading) {
		createCanvas(windowWidth, windowHeight - 69);
		background(130);
		textSize(20);
		rect((windowWidth / 2) - 130, (windowHeight / 2) - 180, 300, 35);
		text("Loading, please wait...", (windowWidth / 2) - 80, (windowHeight / 2)- 155);
	}else {
		createCanvas(windowWidth,windowHeight - 70);
		background(248, 236, 194);
		inputText.position((windowWidth / 2) - 80, (windowHeight / 2) - 113);
	}
}

function keyPressed(){
	if(keyCode == ENTER && a < str.length){
		a = pos;
		toreply();
	} else if (keyCode == ENTER) {
		a = 0;
		pos = 0;
		cnt = 0;
		toreply();
	}
}

function toreply() {
	if (inputText.value().length < 3) {
		output.html("That word is too short!");
	}
	else while (a < str.length) {
		if (str[a].toLowerCase().indexOf(inputText.value().toLowerCase()) != -1) {
			randnum = Math.floor(Math.random() * 2);
			var firstChar = inputText.value()[0];
			var inputCut = inputText.value().substr(1);
			var regex = new RegExp(`"[^"]+?(${firstChar}?${inputCut}).+?"`, "i");
			if(randnum % 2 == 0 && ans != str[a]) {
				ans = str[a];
				while(ans.search(/^[^\w]/) != -1 && ans[0] != '"') {
					ans = ans.replace(/^[^\w]/, '');
				}
				if(regex.exec(ans)){
					output.html(regex.exec(ans)[0]);

				}
				else if (ans[0] == '"' && (ans.replace(/[^"]/g, "").length % 2) == 1) {
					output.html(ans + ".\"");
				}
				else if ((ans.replace(/[^"]/g, "").length % 2) == 1) {
					output.html('"' + ans + '.');
				}
				else {
					output.html(ans + '.');
				}
				ans = str[a];
				return false;
			}
			cnt++;
			ifAns = str[a];
			pos = a + 1;
		}
		a++;
	}
	print(cnt);
	if(pos != 0 && cnt > 1) {
		a = 0;
		pos = 0;
		cnt = 0;
		toreply();
	} else if (cnt == 1){
		while(ifAns.search(/^[^\w]/) != -1 && ifAns[0] != '"') {
			ifAns = ifAns.replace(/^[^\w]/, '');
		}
		if(regex.exec(ifAns)){
			output.html(regex.exec(ifAns)[0]);

		}
		else if (ifAns[0] == '"' && ifAns.replace(/[^"]/g, "").length < 2) {
			output.html(ifAns + ".\"");
		}
		else {
			output.html(ifAns + '.');
		}
		cnt = 0;
	} else if (a >= str.length) {
		output.html("That word or sentence was not found!");
	}
}