#ifndef UIMAIN_H
#define UIMAIN_H

#include <Awesomium/awesomium_capi.h>
#include <Awesomium/KeyboardCodes.h>
#include <windows.h>

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>

class UI;

UI* refToui;

void g_on_key_down(const CL_InputEvent &event, const CL_InputState &state);

void jsCallback (awe_webview *caller, const awe_string *object_name, const awe_string *callback_name, const awe_jsarray *arguments)
{
	char buff[16];
	awe_string_to_utf8(callback_name, buff, awe_string_get_length(callback_name));
	std::string msg ("jsCallback: ");
	msg.append(buff);
	msg.append(" ");
	//awe_jsvalue_to_string(arguments[0]);
	msg.append("\n");
	OutputDebugString (msg.c_str());
}

void jsConsole (awe_webview *caller, const awe_string *message, int line_number, const awe_string *source)
{
	/*char buff[128];
	awe_string_to_utf8(message, buff, awe_string_get_length(message));
	std::string msg ("Java Script Error: ");
	msg.append(buff);
	msg.append("\n");
	OutputDebugString (msg.c_str());*/
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

		//awe_webcore_initialize_default();
		awe_webcore_initialize(true, true, false, awe_string_empty(), awe_string_empty(), awe_string_empty(), awe_string_empty(), awe_string_empty(),
			AWE_LL_NORMAL, false, awe_string_empty(), true,
			awe_string_empty(),awe_string_empty(),awe_string_empty(),awe_string_empty(),awe_string_empty(),awe_string_empty(),
			false, 0, false, false , awe_string_empty());

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

		// Print out the key
		/*std::string msg ("keyPress: ");
		msg.append(event.str.c_str());
		msg.append("\n");
		OutputDebugString (msg.c_str());*/
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
				awe_webview_inject_mouse_down(webView, awe_mousebutton::AWE_MB_LEFT);
			else
				awe_webview_inject_mouse_up(webView, awe_mousebutton::AWE_MB_LEFT);

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
		kbe.type = awe_webkey_type::AWE_WKT_KEYDOWN;
		kbe.text[0] = keyID;
		kbe.text[1] = (wchar_t)0;
		kbe.text[2] = (wchar_t)0;
		kbe.text[3] = (wchar_t)0;
		kbe.modifiers = 0;
		kbe.native_key_code = 0;
		kbe.is_system_key = false;
		awe_webview_inject_keyboard_event(webView, kbe);

		kbe.type = awe_webkey_type::AWE_WKT_CHAR;
		awe_webview_inject_keyboard_event(webView, kbe);

		kbe.type = awe_webkey_type::AWE_WKT_KEYUP;
		awe_webview_inject_keyboard_event(webView, kbe);
	}

	// For keys like backspace
	void simKeyDown(int keyID)
	{
		awe_webkeyboardevent kbe;
		kbe.type = awe_webkey_type::AWE_WKT_KEYDOWN;
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