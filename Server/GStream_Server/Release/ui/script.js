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
		noty({"text":"Hey, are you sure you want to exit?","layout":"bottom","type":"error","animateOpen":{"height":"toggle"},"animateClose":{"height":"toggle"},"speed":500,"timeout":5000,"closeButton":false,"closeOnSelfClick":false,"closeOnSelfOver":false,"modal":true});
	}
	);
	
	// Show the connection menu
	showMenu(CONNECT_MENU);
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