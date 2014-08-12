//Button.c
//
//Copyright 2014 Theo Hussey

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "TG.h"
#include "TG_Button.h"
#include "TG_Text.h"

TG_Button * TG_CreateTextButton(TG_Font * font, int16_t x, int16_t y, int16_t w,
	int16_t h, uint8_t * text, uint32_t color)
{
	TG_Button * button = (TG_Button*)malloc(sizeof(*button));
	
	button->rect.x = x;
	button->rect.y = y;
	button->rect.w = w;
	button->rect.h = h;
	
	button->font = font;
	button->status = TG_BUTTON_RELEASED;
	button->color = color;
	
	button->surface = TG_CreateSurface(w, h);
	
	button->text = (uint8_t*)malloc((strlen(text)+1)*sizeof(uint8_t));
	strcpy(button->text, text);
	
	//Centre the text in the button
	button->textPos.x = (w - TG_TextWidth(font, text))/2;
	//button->textPos.x = 0;
	button->textPos.y = (h - font->charHeight)/2;
	
	//Draw the button background
	TG_DrawRect(button->surface, &((TG_Rect){0,0,button->rect.w, button->rect.h }), color);
	
	//Draw the text onto the button surface
	TG_DrawText(button->surface, font, button->textPos.x, button->textPos.y, text, 0, 0);

	return button;
}

uint8_t TG_UpdateButton(TG_Button * button)
{
	if(TG_RegionHit(&button->rect) && TG_GetMouseDown())
	{	
		if(button->status == TG_BUTTON_RELEASED)
		{
			button->status = TG_BUTTON_JUST_PRESSED;
		}
		else if(button->status == TG_BUTTON_JUST_PRESSED)
		{
			button->status = TG_BUTTON_PRESSED;
		} 
	}
	else
	{
		if(button->status == TG_BUTTON_PRESSED || button->status == TG_BUTTON_JUST_PRESSED)
		{
			button->status = TG_BUTTON_JUST_RELEASED;
		}
		else if(button->status = TG_BUTTON_JUST_RELEASED)
		{
			button->status = TG_BUTTON_RELEASED;
		} 
	}
	
	return button->status;
}

void TG_DrawButton(TG_Button * button, TG_Surface * surface)
{	
	if(button->status == TG_BUTTON_JUST_PRESSED){
		//Draw the button background
		TG_DrawRect(button->surface,
			&((TG_Rect){0,0,button->rect.w, button->rect.h }),
			TG_LightenColor(button->color, -50));
	
		//Draw the text onto the button surface
		TG_DrawText(button->surface, button->font, button->textPos.x, button->textPos.y, button->text, 0, 0);
	}
	else if(button->status == TG_BUTTON_JUST_RELEASED)
	{
		//Draw the button background
		TG_DrawRect(button->surface,
			&((TG_Rect){0,0,button->rect.w, button->rect.h }),
			button->color);
	
		//Draw the text onto the button surface
		TG_DrawText(button->surface, button->font, button->textPos.x, button->textPos.y, button->text, 0, 0);

	}
	
	TG_DrawSurface(button->surface, surface, NULL, button->rect.x,
		button->rect.y);
}

void TG_DestroyButton(TG_Button * b)
{
	TG_FreeSurface(b->surface);
	free(b->text);
	free(b);
}
