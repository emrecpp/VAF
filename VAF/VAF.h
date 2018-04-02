#include "Sound.h"
#pragma once

namespace VAF {

	//using namespace System;
	//using namespace System::ComponentModel;
	//using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace SOUND;
	//using namespace System::Data;
	using namespace System::Drawing;

	public ref class VAF : public System::Windows::Forms::Form
	{
	public:
		VAF(void)
		{
			InitializeComponent();
		}

	protected:
		~VAF()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  lbl100;
	protected:

	private:
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->lbl100 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// lbl100
			// 
			this->lbl100->BackColor = System::Drawing::Color::Transparent;
			this->lbl100->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->lbl100->Font = (gcnew System::Drawing::Font(L"Tahoma", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->lbl100->Location = System::Drawing::Point(20, 94);
			this->lbl100->Name = L"lbl100";
			this->lbl100->Size = System::Drawing::Size(44, 17);
			this->lbl100->TabIndex = 0;
			this->lbl100->Text = L"20";
			this->lbl100->UseCompatibleTextRendering = true;
			this->lbl100->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &VAF::lbl100_MouseClick);
			// 
			// VAF
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::Black;
			this->ClientSize = System::Drawing::Size(60, 120);
			this->ControlBox = false;
			this->Controls->Add(this->lbl100);
			this->ForeColor = System::Drawing::Color::White;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Location = System::Drawing::Point(75, 100);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->MinimumSize = System::Drawing::Size(60, 120);
			this->Name = L"VAF";
			this->ShowIcon = false;
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
			this->Text = L"VAF";
			this->TopMost = true;
			this->Load += gcnew System::EventHandler(this, &VAF::VAF_Load);
			this->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &VAF::VAF_MOUSEWHEEL);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void VAF_Load(System::Object^  sender, System::EventArgs^  e) {
		hwndpencere = (HWND)this->Handle.ToInt32();
		hwndlabel = (HWND)this->lbl100->Handle.ToInt32();
		hdcpencere = GetDC(hwndpencere);

		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)eRefresh, 0, 0, 0);

	}

	private: System::Void VAF_MOUSEWHEEL(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {

		if (!mouse(MOUSEEVENTS::ON_FORM)) {
			SetForegroundWindow(AktifWindowtoback);
			return;
		}

		if (e->Delta > 0)
			SetVolume(GetVolume() + 2);
		else
			SetVolume(GetVolume() - 2);
	}
	private: System::Void lbl100_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {

		if (e->Button == System::Windows::Forms::MouseButtons::Right)//sag click ile cikis
			System::Diagnostics::Process::GetCurrentProcess()->Kill();
		if (lbl100->ForeColor == Color::White)
		{
			*enablemouseclicks4show = true;
			lbl100->ForeColor = Color::OrangeRed;
		}
		else if (lbl100->ForeColor == Color::OrangeRed)
		{
			*enablemouseclicks4show = false;
			lbl100->ForeColor = Color::White;
		}
	}
	};
}
