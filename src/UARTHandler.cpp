#include "UARTHandler.h"

using namespace std;
//Ticker flag_checks;
//
//DigitalOut CTS_pin(PB12);
//InterruptIn RTS_pin(PB11);
//bool ready_to_send;

void EnableInterrupts(bool value){
	if(value){
		NVIC_EnableIRQ(UART1_TX_IRQn);
		NVIC_EnableIRQ(UART1_RX_IRQn);
	} else{
		NVIC_DisableIRQ(UART1_TX_IRQn);
		NVIC_DisableIRQ(UART1_RX_IRQn);
	}
}

//void setRTSTrue() {
//	ready_to_send = true;
//}
//
//void setRTSFalse() {
//	ready_to_send = false;
//}



UARTHandler::UARTHandler(PinName tx, PinName rx, int baud)
{
//	CTS_pin = 0;
//	UART1->IEN |= USART_IF_RXFULL;
//
//	ready_to_send = RTS_pin.read();
//	RTS_pin.rise(&setRTSTrue);
//	RTS_pin.fall(&setRTSFalse);
//
	EnableInterrupts(true);
	NVIC_ClearPendingIRQ(UART1_TX_IRQn);
	NVIC_ClearPendingIRQ(UART1_RX_IRQn);

	serial_ = new RawSerial(tx, rx, baud);

	serial_->attach(callback(this,&UARTHandler::RxISR),RawSerial::RxIrq);
	serial_->attach(callback(this,&UARTHandler::TxISR),RawSerial::TxIrq);

	rx_buffer_ = new Buffer();
	tx_buffer_ = new Buffer();
}

void UARTHandler::RxISR() {
//	if(UART1->IF & USART_IF_RXFULL) {
//		CTS_pin = 1;
//		UART1->IFC = USART_IF_RXFULL;
//	}

	while (UART1->IF & USART_IF_RXDATAV){
		rx_buffer_->data_->at(rx_buffer_->in_) = USART_Rx(UART1);
		rx_buffer_->in_ = (rx_buffer_->in_ + 1) % Buffer::size_;
	}

//	if(!(UART1->IF & USART_IF_RXDATAV))
//		CTS_pin = 0;
}

void UARTHandler::TxISR() {
	while (tx_buffer_->out_ != tx_buffer_->in_) {
		//Todo: this needs a timeout
//		while(RTS_pin.read() == 1){}
		USART_Tx(UART1, tx_buffer_->data_->at(tx_buffer_->out_));
		tx_buffer_->out_ = (tx_buffer_->out_ + 1) % Buffer::size_;
	}
}



void UARTHandler::SendLine(string str){
	EnableInterrupts(false);

	for(auto c : str) {
		if((tx_buffer_->in_ + 1) % Buffer::size_ == tx_buffer_->out_) {
			NVIC_EnableIRQ(UART1_TX_IRQn);
			NVIC_SetPendingIRQ(UART1_TX_IRQn);
			NVIC_EnableIRQ(UART1_RX_IRQn);
			while((tx_buffer_->in_ + 1) % Buffer::size_ == tx_buffer_->out_){}
			NVIC_DisableIRQ(UART1_RX_IRQn);
			NVIC_DisableIRQ(UART1_TX_IRQn);
		}

		tx_buffer_->data_->at(tx_buffer_->in_) = c;
		tx_buffer_->in_ = (tx_buffer_->in_ + 1) % Buffer::size_;
	}

	EnableInterrupts(true);
	NVIC_SetPendingIRQ(UART1_TX_IRQn);
}

int BracketMatch(char c, stack<char> &bracket){
	if(bracket.empty() && c != '{')
		return -1;

	if(c == '}') {
		if(bracket.empty())
			return -1;

		if(bracket.top() == '{')
			bracket.pop();

		return bracket.empty();
	}
	else if(c == '{') {
		bracket.push(c);
	}

	return 0;
}

const string UARTHandler::ReadLine() {
	EnableInterrupts(false);
	string line;
	stack<char> bracket;
	if(rx_buffer_->out_ == rx_buffer_->in_) {
		EnableInterrupts(true);
		while(rx_buffer_->out_ == rx_buffer_->in_){
			wait_us(10);
		}
		EnableInterrupts(false);
	}

	char current_character = rx_buffer_->data_->at(rx_buffer_->out_);
	while(true) {
		if(current_character != '\n' && current_character != '}' && current_character != '{')
			line += current_character;

		if((rx_buffer_->out_ + 1) % Buffer::size_ == rx_buffer_->in_) {
			EnableInterrupts(true);
			while((rx_buffer_->out_ + 1) % Buffer::size_ == rx_buffer_->in_) {
				wait_us(10);
			}
			EnableInterrupts(false);
		}

		rx_buffer_->out_ = (rx_buffer_->out_ + 1) % Buffer::size_;

		int match = BracketMatch(current_character, bracket);

		if(match == 1)
			break;

		if(match == -1 || (current_character == '\n' && line.size() == 1)) {
			line = "ERROR";
			break;
		}

		current_character = rx_buffer_->data_->at(rx_buffer_->out_);
	}

	EnableInterrupts(true);

	rx_buffer_->out_ = rx_buffer_->in_;
	return line;
}

UARTHandler::~UARTHandler()
{
	delete[] rx_buffer_;
	delete[] tx_buffer_;
}

