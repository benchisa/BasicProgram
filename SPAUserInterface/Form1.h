#pragma once
#include "../SPA/SPAController.h"

namespace SPAUserInterface {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::GroupBox^  parserBox;
	protected: 
	private: System::Windows::Forms::Button^  importSource;
	private: System::Windows::Forms::RichTextBox^  parser_textbox;
	private: System::Windows::Forms::OpenFileDialog^  openSimpleSource;
	private: System::Windows::Forms::Button^  parseSource;
	private: System::Windows::Forms::GroupBox^  queryBox;
	private: System::Windows::Forms::Button^  evaluateQuery;
	private: System::Windows::Forms::Button^  importQuery;
	private: System::Windows::Forms::RichTextBox^  query_textbox;
	private: System::Windows::Forms::OpenFileDialog^  openQuery;

	private: System::Windows::Forms::GroupBox^  resultBox;
	private: System::Windows::Forms::RichTextBox^  result_textbox;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->parserBox = (gcnew System::Windows::Forms::GroupBox());
			this->parseSource = (gcnew System::Windows::Forms::Button());
			this->importSource = (gcnew System::Windows::Forms::Button());
			this->parser_textbox = (gcnew System::Windows::Forms::RichTextBox());
			this->openSimpleSource = (gcnew System::Windows::Forms::OpenFileDialog());
			this->queryBox = (gcnew System::Windows::Forms::GroupBox());
			this->evaluateQuery = (gcnew System::Windows::Forms::Button());
			this->importQuery = (gcnew System::Windows::Forms::Button());
			this->query_textbox = (gcnew System::Windows::Forms::RichTextBox());
			this->openQuery = (gcnew System::Windows::Forms::OpenFileDialog());
			this->resultBox = (gcnew System::Windows::Forms::GroupBox());
			this->result_textbox = (gcnew System::Windows::Forms::RichTextBox());
			this->parserBox->SuspendLayout();
			this->queryBox->SuspendLayout();
			this->resultBox->SuspendLayout();
			this->SuspendLayout();
			// 
			// parserBox
			// 
			this->parserBox->Controls->Add(this->parseSource);
			this->parserBox->Controls->Add(this->importSource);
			this->parserBox->Controls->Add(this->parser_textbox);
			this->parserBox->Location = System::Drawing::Point(13, 13);
			this->parserBox->Name = L"parserBox";
			this->parserBox->Size = System::Drawing::Size(335, 204);
			this->parserBox->TabIndex = 0;
			this->parserBox->TabStop = false;
			this->parserBox->Text = L"Parse SIMPLE Source";
			// 
			// parseSource
			// 
			this->parseSource->Location = System::Drawing::Point(224, 50);
			this->parseSource->Name = L"parseSource";
			this->parseSource->Size = System::Drawing::Size(101, 23);
			this->parseSource->TabIndex = 2;
			this->parseSource->Text = L"Parse Source";
			this->parseSource->UseVisualStyleBackColor = true;
			this->parseSource->Click += gcnew System::EventHandler(this, &Form1::parseSource_Click);
			// 
			// importSource
			// 
			this->importSource->Location = System::Drawing::Point(224, 21);
			this->importSource->Name = L"importSource";
			this->importSource->Size = System::Drawing::Size(101, 23);
			this->importSource->TabIndex = 1;
			this->importSource->Text = L"Import Source...";
			this->importSource->UseVisualStyleBackColor = true;
			this->importSource->Click += gcnew System::EventHandler(this, &Form1::importSource_Click);
			// 
			// parser_textbox
			// 
			this->parser_textbox->Location = System::Drawing::Point(7, 21);
			this->parser_textbox->Name = L"parser_textbox";
			this->parser_textbox->Size = System::Drawing::Size(211, 177);
			this->parser_textbox->TabIndex = 0;
			this->parser_textbox->Text = L"procedure Code3{\nwhile a{\nc = c+a;\nd = x+y+z;}\nwhile b{\ny = a;\nb = q+1;}}";
			// 
			// openSimpleSource
			// 
			this->openSimpleSource->FileName = L"simpleSource";
			// 
			// queryBox
			// 
			this->queryBox->Controls->Add(this->evaluateQuery);
			this->queryBox->Controls->Add(this->importQuery);
			this->queryBox->Controls->Add(this->query_textbox);
			this->queryBox->Location = System::Drawing::Point(12, 223);
			this->queryBox->Name = L"queryBox";
			this->queryBox->Size = System::Drawing::Size(336, 204);
			this->queryBox->TabIndex = 1;
			this->queryBox->TabStop = false;
			this->queryBox->Text = L"Evaluate Queries";
			// 
			// evaluateQuery
			// 
			this->evaluateQuery->Location = System::Drawing::Point(232, 51);
			this->evaluateQuery->Name = L"evaluateQuery";
			this->evaluateQuery->Size = System::Drawing::Size(96, 23);
			this->evaluateQuery->TabIndex = 2;
			this->evaluateQuery->Text = L"Evaluate Query";
			this->evaluateQuery->UseVisualStyleBackColor = true;
			this->evaluateQuery->Click += gcnew System::EventHandler(this, &Form1::evaluateQuery_Click);
			// 
			// importQuery
			// 
			this->importQuery->Location = System::Drawing::Point(232, 21);
			this->importQuery->Name = L"importQuery";
			this->importQuery->Size = System::Drawing::Size(96, 23);
			this->importQuery->TabIndex = 1;
			this->importQuery->Text = L"Import Query...";
			this->importQuery->UseVisualStyleBackColor = true;
			this->importQuery->Click += gcnew System::EventHandler(this, &Form1::importQuery_Click);
			// 
			// query_textbox
			// 
			this->query_textbox->Location = System::Drawing::Point(7, 21);
			this->query_textbox->Name = L"query_textbox";
			this->query_textbox->Size = System::Drawing::Size(218, 177);
			this->query_textbox->TabIndex = 0;
			this->query_textbox->Text = resources->GetString(L"query_textbox.Text");
			// 
			// openQuery
			// 
			this->openQuery->FileName = L"queryFile";
			// 
			// resultBox
			// 
			this->resultBox->Controls->Add(this->result_textbox);
			this->resultBox->Location = System::Drawing::Point(363, 13);
			this->resultBox->Name = L"resultBox";
			this->resultBox->Size = System::Drawing::Size(257, 412);
			this->resultBox->TabIndex = 3;
			this->resultBox->TabStop = false;
			this->resultBox->Text = L"Result";
			// 
			// result_textbox
			// 
			this->result_textbox->BackColor = System::Drawing::Color::WhiteSmoke;
			this->result_textbox->Location = System::Drawing::Point(6, 20);
			this->result_textbox->Name = L"result_textbox";
			this->result_textbox->ReadOnly = true;
			this->result_textbox->Size = System::Drawing::Size(245, 386);
			this->result_textbox->TabIndex = 0;
			this->result_textbox->Text = L"";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->ClientSize = System::Drawing::Size(632, 437);
			this->Controls->Add(this->resultBox);
			this->Controls->Add(this->queryBox);
			this->Controls->Add(this->parserBox);
			this->Font = (gcnew System::Drawing::Font(L"Calibri", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Name = L"Form1";
			this->Text = L"Static Program Analyzer";
			this->parserBox->ResumeLayout(false);
			this->queryBox->ResumeLayout(false);
			this->resultBox->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
		// Convert standard string to this string
	private: SPAController *controller;
	private: static bool parse = false;
	private: void MarshalString ( String ^ s, string& os ) {
				 using namespace Runtime::InteropServices;
				 const char* chars = 
					 (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
				 os = chars;
				 Marshal::FreeHGlobal(IntPtr((void*)chars));
			 }



	private: System::Void importSource_Click(System::Object^  sender, System::EventArgs^  e) {
				 StreamReader^ myStream;
				 openSimpleSource->InitialDirectory = Directory::GetCurrentDirectory();
				 openSimpleSource->Filter = "txt files (*.txt)|*.txt|All files(*.*)|*.*";
				 openSimpleSource->FilterIndex = 2;

				 if(openSimpleSource->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				 {
					 myStream = File::OpenText(openSimpleSource->FileName);
					 String^ source = myStream->ReadToEnd();
					 if(source != nullptr){
						 parser_textbox->Text = source;
						 parse = false;
					 }
				 }

			 }
	private: System::Void parseSource_Click(System::Object^  sender, System::EventArgs^  e) {
				 controller = new SPAController();

				 if(parser_textbox->Text != "")
				 {
					 string src;
					 String^ report_msg;

					 MarshalString(parser_textbox->Text, src);
					 string msg = controller->enterProgram(src);

					 report_msg = gcnew String(msg.c_str());
					 result_textbox->Text = report_msg;
					 parse = true;
				 }
				 else
				 {
					 result_textbox->Text = "SIMPLE source is empty.";
				 }
			 }
	private: System::Void importQuery_Click(System::Object^  sender, System::EventArgs^  e) {
				 StreamReader^ myStream;
				 openQuery->InitialDirectory = Directory::GetCurrentDirectory();
				 openQuery->Filter = "txt files (*.txt)|*.txt|All files(*.*)|*.*";
				 openQuery->FilterIndex = 2;

				 if(openQuery->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				 {
					 myStream = File::OpenText(openQuery->FileName);
					 String^ query = myStream->ReadToEnd();
					 if(query != nullptr){
						 query_textbox->Text = query;
					 }
				 }
			 }
	private: System::Void evaluateQuery_Click(System::Object^  sender, System::EventArgs^  e) {
				 if(parse && query_textbox->Text != ""){
					 string src;
					
					 array<String^>^ queries = query_textbox->Text->Split('\n');
					 for(int i = 0; i < queries->Length; i++){
						  string query;
						  String^ report_msg;
						  
						  MarshalString(queries[i], query);
						  list<string> results = controller->getQueryResult(query);
						  list<string>::iterator itr;
						  report_msg += queries[i] + "\n";
						  if(results.empty()) report_msg += "None  ";
						  for(itr = results.begin(); itr!=results.end();itr++){
							  string msg = *itr;
							  report_msg += gcnew String(msg.c_str());
							  report_msg += ", ";
						  }
						  report_msg = report_msg->Substring(0, report_msg->Length-2) + "\n";
						  result_textbox->Text = report_msg;
					 }
				}
				 else if(!parse){
					 result_textbox->Text = "SIMPLE Source not parsed.";
				 }
				 else{
					  result_textbox->Text = "Query is empty.";
				 }
			 }
	};
}

