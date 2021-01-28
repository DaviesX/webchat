import tensorflow as tf
import numpy as np

if __name__ == "__main__":
    model = tf.saved_model.load(
        export_dir="/home/davis/saved_models/gomoku_cnn_shared_tower_11_11_b4")

    board_features = tf.constant(
        value=[[[0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0],
                [0, 0, 0, 1, -1, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 1, 1, 1, -1, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]], 
        dtype=tf.float32)
    
    game_phase_place_3_stones = tf.constant(value=[0], dtype=tf.float32)
    game_phase_swap2_decision = tf.constant(value=[0], dtype=tf.float32)
    game_phase_place2_more_stones = tf.constant(value=[0], dtype=tf.float32)
    game_phase_stone_type_decision = tf.constant(value=[0], dtype=tf.float32)
    game_phase_standard_gomoku = tf.constant(value=[1], dtype=tf.float32)

    next_move_stone_type = tf.constant(value=[-1], dtype=tf.float32)

    policy, value, _ = model(board_features,
                             game_phase_place_3_stones,
                             game_phase_swap2_decision,
                             game_phase_place2_more_stones,
                             game_phase_stone_type_decision,
                             game_phase_standard_gomoku,
                             next_move_stone_type)

    print("policy=\n{0}".format(policy.numpy()))
    i = np.argmax(policy.numpy())
    x = i % 11
    y = i // 11
    print(i, x, y)
    print("value=\n{0}".format(value.numpy()))
