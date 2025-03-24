#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>

// =======================
// 음계 정의 (CTC 주기 값)
// =======================
#define NOTE_C5  1911
#define NOTE_D5  1703
#define NOTE_E5  1517
#define NOTE_F5  1432
#define NOTE_G5  1276
#define NOTE_A5  1136
#define NOTE_B5  1012
#define NOTE_C6  956
#define REST     0

// =======================
// 멜로디: 작은별
// =======================
const uint16_t twinkle_notes[] = {
	NOTE_C5, NOTE_C5, NOTE_G5, NOTE_G5, NOTE_A5, NOTE_A5, NOTE_G5,
	NOTE_F5, NOTE_F5, NOTE_E5, NOTE_E5, NOTE_D5, NOTE_D5, NOTE_C5
};

const uint8_t twinkle_beats[] = {
	4, 4, 4, 4, 4, 4, 2,
	4, 4, 4, 4, 4, 4, 2
};

// =======================
// 멜로디: 비행기
// =======================
const uint16_t plane_notes[] = {
	NOTE_E5, NOTE_E5, NOTE_F5, NOTE_G5,
	NOTE_G5, NOTE_F5, NOTE_E5, NOTE_D5,
	NOTE_C5, NOTE_C5, NOTE_D5, NOTE_E5,
	NOTE_E5, NOTE_D5, NOTE_D5
};

const uint8_t plane_beats[] = {
	4, 4, 4, 4,
	4, 4, 4, 4,
	4, 4, 4, 4,
	4, 4, 2
};

// =======================
// 멜로디: 엘리제를 위하여 (첫 8마디)
// =======================
const uint16_t Elise_Tune[] = {
	NOTE_C5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_C5, NOTE_G5, NOTE_A5, NOTE_G5
};

const uint8_t Elise_Beats[] = {
	4, 4, 4, 4, 4, 4, 4, 4
};


#define TWINKLE_LEN (sizeof(twinkle_notes) / sizeof(twinkle_notes[0]))
#define PLANE_MELODY_LEN  (sizeof(plane_notes) / sizeof(plane_notes[0]))

// =======================
// ENUM - 사이렌 상태
// =======================
typedef enum {
	SIREN_IDLE,
	SIREN_AMBULANCE_UP,
	SIREN_AMBULANCE_DOWN
} SpeakerFSMState;

// =======================
// 전역 변수
// =======================
volatile uint16_t speaker_msec = 0;
int sound_state = 0; // 0: 무음, 1: 사이렌, 2: 작은별, 3: 비행기
extern volatile uint8_t button_state;

// =======================
// 초기화 함수
// =======================
void init_speaker(void)
{
	DDRE |= 0x08;  // OC3A 출력
	TCCR3A = (1 << COM3A0);  // 비교일치 시 토글
	TCCR3B = (1 << WGM32) | (1 << CS31);  // CTC 모드, 분주 8
	TCCR3C = 0;
	OCR3A = 1000;
	ETIMSK |= (1 << OCIE3A);  // Timer3 비교일치 인터럽트 허용
}

// =======================
// 인터럽트
// =======================
ISR(TIMER3_COMPA_vect)
{
	speaker_msec++;
}

// =======================
// 상태 전이 함수
// =======================
void sound_transition(void)
{
	sound_state = (sound_state + 1) % 4;  // 0 -> 1 -> 2 -> 3 -> 0
}

// =======================
// 사운드 FSM (사이렌 + 멜로디)
// =======================
void process_siren(void)
{
	static uint16_t prev_msec = 0;
	static SpeakerFSMState state = SIREN_IDLE;
	static uint16_t tone = 0;

	static uint8_t melody_index = 0;
	static uint16_t note_duration = 0;

	if (speaker_msec - prev_msec < 10) return;
	prev_msec = speaker_msec;

	switch (sound_state)
	{
		case 1:  // 사이렌 (앰뷸런스)
		TCCR3A |= (1 << COM3A0);
		if (state == SIREN_IDLE) { tone = 900; state = SIREN_AMBULANCE_UP; }

		if (state == SIREN_AMBULANCE_UP) {
			tone += 10;
			if (tone >= 1900) state = SIREN_AMBULANCE_DOWN;
			} else if (state == SIREN_AMBULANCE_DOWN) {
			tone -= 10;
			if (tone <= 900) state = SIREN_AMBULANCE_UP;
		}
		OCR3A = tone;
		break;

		case 2:  // 사이렌 (경찰차)
		{
			TCCR3A |= (1 << COM3A0); // 출력 활성화

			static uint16_t tone = 800; // 시작 톤
			static SpeakerFSMState state = SIREN_IDLE;

			if (state == SIREN_IDLE) {
				tone = 800; // 초기 톤 설정
				state = SIREN_AMBULANCE_UP; // 상태 전환
			}

			if (state == SIREN_AMBULANCE_UP) {
				tone += 15; // 빠르게 톤 상승
				if (tone >= 1500) { // 상한값
					state = SIREN_AMBULANCE_DOWN; // 상태 전환
				}
				} else if (state == SIREN_AMBULANCE_DOWN) {
				tone -= 15; // 빠르게 톤 하강
				if (tone <= 800) { // 하한값
					state = SIREN_AMBULANCE_UP; // 상태 전환
				}
			}

			OCR3A = tone; // 톤 출력
		}
		break;

		
		case 3:		// 동요 (작은별)
		TCCR3A |= (1 << COM3A0);
		if (note_duration == 0) {
			tone = twinkle_notes[melody_index];
			note_duration = twinkle_beats[melody_index] * 100;

			if (tone == REST)
			TCCR3A &= ~(1 << COM3A0);
			else {
				TCCR3A |= (1 << COM3A0);
				OCR3A = tone;
			}

			melody_index++;
			if (melody_index >= TWINKLE_LEN)
			melody_index = 0;
			} else {
			note_duration -= 10;
		}
		break;
		
		
		case 4:		//자동차 후진 소리 (엘리제를 위하여)
		{
			static uint8_t melody_index = 0;
			static uint16_t note_duration = 0;

			if (note_duration == 0) {
				tone = Elise_Tune[melody_index];
				note_duration = Elise_Beats[melody_index];

				if (tone == 0) { // REST (무음)
					TCCR3A &= ~(1 << COM3A0);
					} else {
					TCCR3A |= (1 << COM3A0);
					OCR3A = tone;
				}

				melody_index++;
				if (melody_index >= sizeof(Elise_Tune) / sizeof(Elise_Tune[0])) {
					melody_index = 0; // 8마디 끝나면 멜로디 반복
				}
				} else {
				note_duration -= 10; // 10ms 단위로 감소
			}
		}

		break;
		
		default:  // 무음
			TCCR3A &= ~(1 << COM3A0);
			OCR3A = 1000;
			state = SIREN_IDLE;
			note_duration = 0;
			melody_index = 0;
		break;
	}
}