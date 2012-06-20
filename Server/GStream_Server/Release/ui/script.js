//===============================================
// Ui functions
//===============================================

// Set up the event handlers
function setup()
{
	// Server connect
	$("#btt_serverConnect").click(serverConnect);
	
	// Overlay menu options
	$("#btt_resume").click(function () {  showMenu(HIDE_ALL); });
	$("#btt_gameslist").click(function () {  showMenu(GAME_LIST_MENU); });
	$("#btt_settings").click(function () {  showMenu(SETTINGS_MENU); });
	$("#btt_console").click(function () {  showMenu(CONSOLE_MENU); });
	$("#btt_exit").click(function () 
	{
		ui_logInfo("Nupe no exits yet :D muhahahahahahaahahhahaha");
		//noty({"text":"Hey, are you sure you want to exit?","layout":"bottom","type":"error","animateOpen":{"height":"toggle"},"animateClose":{"height":"toggle"},"speed":500,"timeout":5000,"closeButton":false,"closeOnSelfClick":false,"closeOnSelfOver":false,"modal":true});
	}
	);
	
	//GSTREAM.test();
	
	// Show the connection menu
	showMenu(CONNECT_MENU);
	
	// Set the height of the console output div
	$("#consoleoutput").height(($("#console").height() - $("#connectBoxHeader").height()) - 20);
	$("#gamesListContent").height(($("#gamesList").height() - $("#connectBoxHeader").height()) - 20);
	
	addGame("Halo","", 1);
	addGame("Killing Floor","", 1);
	addGame("L4D 2","", 1);
	addGame("Recettear: An Item Shop's Tale","", 1);
}

var HIDE_ALL = -1;
var CONNECT_MENU = 0;
var GAME_LIST_MENU = 1;
var OVERLAY_MENU = 2;
var SETTINGS_MENU = 3;
var CONSOLE_MENU = 4;
function showMenu(menu)
 {
	var state = 'visible';
	
	$('.menu').css('visibility','hidden');
	
	if (menu == CONNECT_MENU)
	{
		$('#connectionMenu').css('visibility',state);
	}
	else if (menu == GAME_LIST_MENU)
	{
		$('#gamesListMenu').css('visibility',state);
	}
	else if (menu == OVERLAY_MENU)
	{
		$('#overlayMenu').css('visibility',state);
	}
	else if (menu == SETTINGS_MENU)
	{
		$('#settingsMenu').css('visibility',state);
	}
	else if (menu == CONSOLE_MENU)
	{
		$('#consoleMenu').css('visibility',state);
	}
 }
 
// Display a connection error message
function connectionError(msg)
{
	noty(
	{
	"text":"<center>Connection Error:</center></br> " + msg,
	"layout":"topRight",
	"type":"error",
	"animateOpen":{"height":"toggle"},
	"animateClose":{"height":"toggle"},
	"speed":500,
	"timeout":5000,
	"closeButton":false,
	"closeOnSelfClick":true,
	"closeOnSelfOver":false,
	"modal":false}
	);
	//$("#connectionError").html("<b>"+ msg +"</b></br>");
}

function serverConnect()
{
	// Get the info
	var ip = $("#tb_ip").val();
	var port = parseInt($("#tb_port").val());
	var pass = $("#tb_pass").val();
	
	// Check em
	if (ip == "")
	{
		connectionError("Erm, I kinda need a server IP");
	}
	if (port > 65535 || port < 0)
	{
		connectionError("Ah, thats an Invaild port number :/");
	}
	
	// c++ callback
	//GSTREAM.test();
	
	showMenu(OVERLAY_MENU);
	
	
}

// Games list
//===============================

function addGame(title, boxArt, id)
{
	$("#gamesListContent").html($("#gamesListContent").html() +
	'<div id="gi_'+  id +'" class="gameItem">' +
		'<div class="gameItemTitle">'+  title +'</div>'+
	'</div>'
	);
	
	bindGameItemAnni();
}

function textWidth(text)
{
 var calc = '<span style="display:none">' + text + '</span>';
 $('body').append(calc);
 var width = $('body').find('span:last').width();
 $('body').find('span:last').remove();
 return width;
};


// Animate the game Icons
function bindGameItemAnni()
{
	$(".gameItem").hover(
	function () 
	{

		//alert("Text Pixel Len: " + textWidth($(this).children(".gameItemTitle").html()));
		$(this).children(".gameItemTitle").css('visibility', 'visible');
		$(this).children(".gameItemTitle").animate(
		{
			top: "-=30"
		}
		, 200); 
	}
	,
	function () 
	{	
		$(this).children(".gameItemTitle").animate(
		{
			top: "+=30"
		}
		, 200, function() { $(this).css('visibility','hidden'); }); 
	}
	);
	
	// Center the game items
	var glstWidth = $("#gamesListContent").outerWidth();
	var gitmWidth = $(".gameItem").outerWidth(true);
	var numPerRow = Math.floor(glstWidth / gitmWidth);
	var marginWidth = (glstWidth - (gitmWidth * numPerRow)) / 2;
	
	if ($(".gameItem").length < numPerRow)
		marginWidth = ((glstWidth - (gitmWidth * $(".gameItem").length)) / 2);
	
	/*
	ui_logInfo("Contener Width: " + glstWidth);
	ui_logInfo("Item Width: " + gitmWidth);
	ui_logInfo("NumPerRow: " + numPerRow);
	ui_logInfo("Count: " + $(".gameItem").length);
	ui_logInfo("Margin Width: " + marginWidth);
	ui_logInfo("=====================================================================");
	*/
	
	$("#gamesListContent").css("padding-left", marginWidth);
}

// Console Loggging
//===============================

function ui_logInfo(msg)
{
	var conLog = $('#consoleoutput').html();
	$('#consoleoutput').html(conLog + '<div class="conInfo"> [Info] ' + msg + "</div>");
}

function ui_logWarn(msg)
{
	var conLog = $('#consoleoutput').html();
	$('#consoleoutput').html(conLog + '<div class="conWarn"> [Warn] ' + msg + "</div>");
}

function ui_logError(msg)
{
	var conLog = $('#consoleoutput').html();
	$('#consoleoutput').html(conLog + '<div class="conError"> [Error] ' + msg + "</div>");
}