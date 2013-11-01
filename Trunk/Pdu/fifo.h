#ifndef FIFO_H
#define FIFO_H

enum FIFO_ORDER_FLAG{
	FIFO_ORDER_R_BEFORE_W = 1,
	FIFO_ORDER_W_BEFORE_R = 2
};

class Fifo{
private:
	static const int SIZE = 20;
	unsigned char data[SIZE];
	unsigned int rPosition;
	unsigned int wPosition;
	
public:
	FIFO_ORDER_FLAG order;

	unsigned int getAvailableWriteSize(){
		if (order == FIFO_ORDER_W_BEFORE_R)
			return rPosition - wPosition;
		else
			return rPosition + SIZE - wPosition;
	}

public:
	Fifo()
		:	rPosition(0), wPosition(0), order(FIFO_ORDER_R_BEFORE_W){
	}

	~Fifo(){
	}

	bool put(unsigned char value){
		if (order == FIFO_ORDER_W_BEFORE_R)
			if (wPosition == rPosition)
				return false;

		data[wPosition] = value;
		wPosition++;
		if (wPosition == SIZE){
			wPosition = 0;
			order = FIFO_ORDER_W_BEFORE_R;
		}
		return true;
	}

	unsigned int get(unsigned char* pData){
		int count = 1;
		if (order == FIFO_ORDER_R_BEFORE_W){
			if (wPosition == rPosition)
				return 0;
			else{
				unsigned int rSize = wPosition - rPosition;

				if (count > rSize)
					count = rSize;

				*pData = data[rPosition];
				rPosition += count;
				return count;
			}
		}

		unsigned int rSize = SIZE - rPosition;

		if (count <= rSize){
			*pData = data[rPosition];

			rPosition += count;
			if (rPosition == SIZE){
				rPosition = 0;
				order = FIFO_ORDER_R_BEFORE_W;
			}

			return count;
		}

		*pData = data[rPosition];

		unsigned int dopCount = 0;
		if (count - rSize > wPosition)
			dopCount = wPosition;
		else
			dopCount = count - rSize;

		*(pData + rSize) = data[dopCount];

		rPosition = dopCount;
		order = FIFO_ORDER_R_BEFORE_W;

		return rSize + dopCount;
	}

	bool isEmpty(){
		if (order == FIFO_ORDER_R_BEFORE_W)
			return (rPosition == wPosition);
		else
			return false;
	}

	void clear(){
		rPosition = 0;
		wPosition = 0;
		order = FIFO_ORDER_R_BEFORE_W;
	}

	unsigned int getDataSize(){
		if (order == FIFO_ORDER_R_BEFORE_W)
			return (wPosition - rPosition);
		else
			return SIZE - rPosition + wPosition;
	}
};
#endif
