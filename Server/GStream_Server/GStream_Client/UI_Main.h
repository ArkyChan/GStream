#ifndef UIMAIN_H
#define UIMAIN_H

#include <Awesomium/awesomium_capi.h>
#include <Awesomium/KeyboardCodes.h>
#include <windows.h>

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>

// UI loging levels
enum UI_ERROR_LEVEL 
{
	UIE_INFO,
	UIE_WARN,
	UIE_ERROR
};

//#define UI_GOBAL_CSS "::-webkit-scrollbar {width: 12px; height: 12px; background-color: #f2f2f1; } ::-webkit-scrollbar-track { border-radius: 10px; border: 1px solid #bbb7b3; background: -webkit-gradient(linear, left top, right top, color-stop(0, #dcd9d7), color-stop(1, #e5e3e2)); } ::-webkit-scrollbar-thumb {  border-radius: 10px; -webkit-box-shadow: inset 1px 0 0 1px white; border: 1px solid #9c9996; background: -webkit-gradient(linear, left top, right top, color-stop(0, #f9f9f8), color-stop(1, #e6e4e3)); } ::-webkit-scrollbar-track-piece:disabled { display: none !important; } ::-webkit-scrollbar-track:disabled { margin: 6px; }"
#define UI_GOBAL_CSS ""

class UI;

UI* refToui;

void g_on_key_down(const CL_InputEvent &event, const CL_InputState &state);

char* awe_string_toChar(const awe_string * aweStr)
{
	int len = awe_string_get_length(aweStr);
	char *buff = (char*)malloc((len+1)*sizeof(char));
	awe_string_to_utf8(aweStr, buff, len);
	buff[len] = '\0';

	return buff;
}

// Decide what to do when javascripts calls us
void jsCallback (awe_webview *caller, const awe_string *object_name, const awe_string *callback_name, const awe_jsarray *arguments)
{
	char* aweObjectName = awe_string_toChar(object_name);
	char* aweCallbackName = awe_string_toChar(callback_name);

	std::string msg ("jsCallback: ");
	msg.append(aweObjectName);
	msg.append(" Object name: ");
	msg.append(aweCallbackName);
	msg.append("\n");
	OutputDebugString (msg.c_str());

	free(aweObjectName);
	free(aweCallbackName);
}

// Oh noes javascripts gotten an error
void jsConsole (awe_webview *caller, const awe_string *message, int line_number, const awe_string *source)
{
	char* aweMsg = awe_string_toChar(message);

	std::string msg ("Java Script Error: ");
	msg.append(aweMsg);
	msg.append("\n");
	OutputDebugString (msg.c_str());

	free(aweMsg);
}

// Calls a javaScript function
// Leave object empty for Gobal scope and frame empty for the main frame :)
void callJSFunc(awe_webview * view, char* object, char* function, const awe_jsarray * args, char* frame)
{
	awe_string* objStr = awe_string_create_from_ascii(object, strlen(object));
	awe_string* funcStr = awe_string_create_from_ascii(function, strlen(function));
	awe_string* frameStr = awe_string_create_from_ascii(frame, strlen(frame));

	awe_webview_call_javascript_function(view, objStr, funcStr, args, frameStr);

	awe_string_destroy(objStr);
	awe_string_destroy(funcStr);
	awe_string_destroy(frameStr);
}

class UI
{
public:
	// Create the ui instance with the given size and a start url, the base dir is where it will look for local files
	UI (CL_DisplayWindow* window, int w, int h, const char* baseDIR, const char* url)
	{
		refToui = this;

		renderWindow = window;
		width = w;
		height = h;
		hasFocus = true;

		mouse = window->get_ic().get_mouse();
		keyboard = window->get_ic().get_keyboard();
	
		slot_key_down = keyboard.sig_key_down().connect(&g_on_key_down);

		pbuff = CL_PixelBuffer (width, height, cl_argb8);
		//img = CL_Image (renderWindow->get_gc(), pbuff, pbuff.get_size());

		awe_string* cssStr = awe_string_create_from_ascii(UI_GOBAL_CSS, strlen(UI_GOBAL_CSS));
		//awe_webcore_initialize_default();
		awe_webcore_initialize(true, true, false, awe_string_empty(), awe_string_empty(), awe_string_empty(), awe_string_empty(), awe_string_empty(),
			AWE_LL_NORMAL, false, awe_string_empty(), true,
			awe_string_empty(),awe_string_empty(),awe_string_empty(),awe_string_empty(),awe_string_empty(),awe_string_empty(),
			false, 0, false, false , cssStr);

		awe_string_destroy(cssStr);

		webView = awe_webcore_create_webview(width, height, false);

		awe_webview_set_transparent(webView, true);

		// Set the base DIR
		awe_string* dir_str = awe_string_create_from_ascii(baseDIR, strlen(baseDIR));
		awe_webcore_set_base_directory(dir_str);
		awe_string_destroy(dir_str);

		awe_webview_set_callback_js_console_message(webView, &jsConsole);

		// Create the javascript bindings
		awe_string* tempStr = awe_string_create_from_ascii("GSTREAM", strlen("GSTREAM"));
		awe_webview_create_object(webView, tempStr);
		awe_webview_set_object_callback(webView, tempStr, awe_string_create_from_ascii("test", strlen("test")));
		awe_webview_set_callback_js_callback(webView, &jsCallback);
		//awe_webview_set_callback_js_callback(webView, );
		awe_string_destroy(tempStr);

		loadURL(url);

		setFoucus(true);
	}

	void loadURL(const char* url)
	{
		awe_string* url_str = awe_string_create_from_ascii(url, strlen(url));

		awe_webview_load_file(webView, url_str, awe_string_empty());

		awe_string_destroy(url_str);
	}

	// Save a screenshot of the current UI
	void screenShot(char* fileName = "ui_snapshot.jpg")
	{
        awe_string* filename_str = awe_string_create_from_ascii(fileName, strlen(fileName));

        awe_renderbuffer_save_to_jpeg(renderBuffer, filename_str, 90);

        awe_string_destroy(filename_str);
	}

	// Should the UI respond to inputs?
	void setFoucus(bool focus)
	{
		if (focus)
			awe_webview_focus(webView);
		else
			awe_webview_unfocus(webView);

		hasFocus = focus;
	}

	void resize(int w, int h)
	{
		width = w;
		height = h;

		awe_webview_resize(webView, w, h, false, 15);
	}

	// Log info to the console window in the ui
	void logInfo(UI_ERROR_LEVEL level, char* msg)
	{
		// Some how i feel this should be simpler and nicer :/
		const awe_jsvalue* args[1];

		awe_string* msgStr = awe_string_create_from_ascii(msg, strlen(msg));
		args[0] = awe_jsvalue_create_string_value(msgStr);

		awe_jsarray* arr = awe_jsarray_create(args, 1);

		if (level == UIE_INFO)
			callJSFunc(webView, "", "ui_logInfo", arr, "");
		else if (level == UIE_WARN)
			callJSFunc(webView, "", "ui_logWarn", arr, "");
		else if (level == UIE_ERROR)
			callJSFunc(webView, "", "ui_logError", arr, "");

		awe_string_destroy(msgStr);
		//awe_jsvalue_destroy(args);
		awe_jsarray_destroy(arr);
	}

	void render()
	{
		// Get input
		getInputs();

		// Update the webpage
		awe_webcore_update();

		//if (awe_webview_is_dirty(webView))
			renderBuffer = awe_webview_render(webView);

		//pbuff.upload_data(CL_Rectf(0.0f, 0.0f, pbuff.get_width(), pbuff.get_height()), awe_renderbuffer_get_buffer(renderBuffer));
		//img.set_alpha(0.1);
		//img.draw(renderWindow->get_gc(), 0, 0);

		glDrawPixels(width, height, GL_BGRA, GL_UNSIGNED_BYTE, awe_renderbuffer_get_buffer(renderBuffer));
	}

	void on_key_down(const CL_InputEvent &event, const CL_InputState &state)
	{
		if (event.id == CL_KEY_BACKSPACE)
			simKeyDown(8);
		else if (event.id == CL_KEY_RETURN)
			simKeyDown(13);
		else if (event.id == CL_KEY_TAB)
			simKeyDown(9);
		else if (event.id == CL_KEY_ESCAPE)
			simKeyDown(27);

		simKeyPress(event.str.c_str()[0]);
	}

	~UI()
	{
		// Destroy our WebView instance
		awe_webview_destroy(webView);

		// Destroy our WebCore instance
		awe_webcore_shutdown();
	}

private:
	awe_webview* webView;
	const awe_renderbuffer* renderBuffer; // The rendered webpage
	CL_DisplayWindow* renderWindow;
	CL_InputDevice mouse;
	CL_InputDevice keyboard;
	CL_Slot slot_key_down;

	int width, height;
	bool hasFocus;

	CL_PixelBuffer pbuff;
	CL_Image img;

	void getInputs()
	{
		if (hasFocus)
		{
			awe_webview_inject_mouse_move(webView, mouse.get_x(), mouse.get_y());

			if (mouse.get_keycode(CL_MOUSE_LEFT))
				awe_webview_inject_mouse_down(webView, AWE_MB_LEFT);
			else
				awe_webview_inject_mouse_up(webView, AWE_MB_LEFT);

			// Relaod the page
			if (mouse.get_keycode(CL_MOUSE_RIGHT))
			{
				awe_webview_reload(webView);
			}

		}
	}

	// For most keys and letters
	void simKeyPress(int keyID)
	{
		awe_webkeyboardevent kbe;
		kbe.type = AWE_WKT_KEYDOWN;
		kbe.text[0] = keyID;
		kbe.text[1] = (wchar_t)0;
		kbe.text[2] = (wchar_t)0;
		kbe.text[3] = (wchar_t)0;
		kbe.modifiers = 0;
		kbe.native_key_code = 0;
		kbe.is_system_key = false;
		awe_webview_inject_keyboard_event(webView, kbe);

		kbe.type = AWE_WKT_CHAR;
		awe_webview_inject_keyboard_event(webView, kbe);

		kbe.type = AWE_WKT_KEYUP;
		awe_webview_inject_keyboard_event(webView, kbe);
	}

	// For keys like backspace
	void simKeyDown(int keyID)
	{
		awe_webkeyboardevent kbe;
		kbe.type = AWE_WKT_KEYDOWN;
		kbe.virtual_key_code = keyID;
		kbe.native_key_code = 0;
		kbe.modifiers = 0;
		kbe.is_system_key = false;
		awe_webview_inject_keyboard_event(webView, kbe);
	}
};

void g_on_key_down(const CL_InputEvent &event, const CL_InputState &state)
{
	refToui->on_key_down(event, state);
}

#endif