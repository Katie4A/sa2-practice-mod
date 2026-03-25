
#include "hunting_utils.h"

void setPieceByID(int id) {
	//PrintDebug("Setting Piece ID %X", id & 0xFFFF);
	byte major_id = id & 0xFF;
	Emerald curr_piece;
	Emerald* slot_arr = (Emerald*)0;
	int slot_arr_len = 0;
	switch (major_id) {
	case 0:
	case 2:
	case 5:
		slot_arr = EmeraldManager->slot_2_emeralds;
		slot_arr_len = EmeraldManager->slot_2_array_len;
		break;
	case 1:
	case 3:
		slot_arr = EmeraldManager->slot_1_emeralds;
		slot_arr_len = EmeraldManager->slot_1_array_len;
		break;
	case 4:
	case 7:
	case 8:
		slot_arr = EmeraldManager->slot_3_emeralds;
		slot_arr_len = EmeraldManager->slot_3_array_len;
		break;
	case 0xA:
		slot_arr = EmeraldManager->slot_enemy_emeralds;
		slot_arr_len = EmeraldManager->slot_enemy_array_len;
		break;
	}
	if (slot_arr_len == 0) {
		PrintDebug("PIECE %X HAS INVALID ID", id);
		return;
	}
	for (int i = 0; i < slot_arr_len; i++) {
		curr_piece = slot_arr[i];
		if (curr_piece.id == id) {
			if (slot_arr == EmeraldManager->slot_2_emeralds) {
				EmeraldManager->piece_2.id = curr_piece.id;
				EmeraldManager->piece_2.position.x = curr_piece.position.x;
				EmeraldManager->piece_2.position.y = curr_piece.position.y;
				EmeraldManager->piece_2.position.z = curr_piece.position.z;
				//PrintDebug("P2 STORED ID: %X",EmeraldManager->piece_2.id);
				return;
			}
			else if (slot_arr == EmeraldManager->slot_3_emeralds) {
				EmeraldManager->piece_3.id = curr_piece.id;
				EmeraldManager->piece_3.position.x = curr_piece.position.x;
				EmeraldManager->piece_3.position.y = curr_piece.position.y;
				EmeraldManager->piece_3.position.z = curr_piece.position.z;
			}
			else if (slot_arr == EmeraldManager->slot_1_emeralds || slot_arr == EmeraldManager->slot_enemy_emeralds) {
				EmeraldManager->piece_1.id = curr_piece.id;
				EmeraldManager->piece_1.position.x = curr_piece.position.x;
				EmeraldManager->piece_1.position.y = curr_piece.position.y;
				EmeraldManager->piece_1.position.z = curr_piece.position.z;
			}
		}
	}
	if (slot_arr_len > 0) {
		PrintDebug("PIECE %X NOT FOUND", id);
	}

