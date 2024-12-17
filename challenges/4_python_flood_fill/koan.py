#!/bin/env python

# The task is to complete the implementation of the ConnectedRegionPainter class.
# The class is used to flood fill a region of the image with a new color: starting at the
# target pixel the method should change the color of the target pixel and all adjacent
# pixels of the same color to the new color. For this exercise, we will consider only the
# 4-connected pixels (up, down, left, right) and not the diagonals.
# The image is represented as a list of lists, where each list represents a row of the image.

import unittest

# BEGIN EDIT ------------------------------------------------------


class ConnectedRegionPainter:

    # constructor to create image from a list of lists, a width and a height
    def __init__(self, image, width, height):
        self.image = image
        self.width = width
        self.height = height
        # validate that width and height match the image
        if len(image) != height:
            raise ValueError("Invalid height")
        for row in image:
            if len(row) != width:
                raise ValueError("Invalid width")

    # method to get the color of a pixel
    def get_pixel(self, x, y):
        return None

    # method to set the color of a pixel
    def set_pixel(self, x, y, color):
        pass

    # method to get the image back
    def get_image(self):
        return None

    # method to determine if the pixels are within the image
    def is_valid_pixel(self, x, y):
        return False

    # method to flood fill a region with a new color
    def flood_fill(self, x, y, new_color):
        pass


# END EDIT --------------------------------------------------------


class PaintTest(unittest.TestCase):

    def setUp(self):
        self.TEST_IMAGE = [
            # 0, 1, 2, 3, 4, 5, 6
            [0, 0, 0, 0, 0, 0, 0],  # 0
            [0, 1, 2, 1, 3, 1, 0],  # 1
            [0, 2, 2, 2, 3, 1, 0],  # 2
            [0, 1, 2, 1, 3, 1, 0],  # 3
            [0, 3, 3, 3, 3, 1, 0],  # 4
            [0, 1, 1, 1, 1, 3, 0],  # 5
        ]
        self.WIDTH = 7
        self.HEIGHT = 6

    def tearDown(self):
        pass

    def test_trivial_functions_square(self):
        image = [
            # 0  1  2  3  4  5  6
            [0, 0, 0, 0, 0, 0, 0],  # 0
            [0, 1, 1, 1, 1, 1, 0],  # 1
            [0, 1, 1, 1, 1, 1, 0],  # 2
            [0, 1, 1, 1, 1, 1, 0],  # 3
            [0, 1, 1, 1, 1, 1, 0],  # 4
            [0, 1, 1, 1, 1, 1, 0],  # 5
            [0, 0, 0, 0, 0, 0, 0],  # 6
        ]
        di = ConnectedRegionPainter(image, 7, 7)
        self.assertEqual(di.get_pixel(0, 0), 0)
        self.assertEqual(di.get_pixel(1, 1), 1)
        self.assertEqual(di.get_pixel(5, 5), 1)
        self.assertEqual(di.get_pixel(6, 6), 0)
        self.assertTrue(di.is_valid_pixel(0, 0))
        self.assertTrue(di.is_valid_pixel(6, 6))
        self.assertFalse(di.is_valid_pixel(7, 7))
        self.assertFalse(di.is_valid_pixel(-1, -1))

    def test_trivial_functions_rect(self):
        image = [
            [0, 0, 0, 0, 0, 0, 2],
            [1, 0, 0, 0, 0, 0, 3],
        ]

        di = ConnectedRegionPainter(image, 7, 2)
        self.assertEqual(di.get_pixel(0, 0), 0)
        self.assertEqual(di.get_pixel(1, 1), 0)
        self.assertEqual(di.get_pixel(6, 1), 3)
        self.assertEqual(di.get_pixel(6, 0), 2)
        self.assertTrue(di.is_valid_pixel(0, 0))
        self.assertTrue(di.is_valid_pixel(6, 1))
        self.assertFalse(di.is_valid_pixel(7, 2))
        self.assertFalse(di.is_valid_pixel(-1, -1))

    def test_flood_fill_1(self):
        di = ConnectedRegionPainter(self.TEST_IMAGE, self.WIDTH, self.HEIGHT)
        di.flood_fill(0, 0, 1)

        expected = [
            [1, 1, 1, 1, 1, 1, 1],
            [1, 1, 2, 1, 3, 1, 1],
            [1, 2, 2, 2, 3, 1, 1],
            [1, 1, 2, 1, 3, 1, 1],
            [1, 3, 3, 3, 3, 1, 1],
            [1, 1, 1, 1, 1, 3, 1],
        ]
        self.assertEqual(di.get_image(), expected)

    def test_flood_fill_single_pixel_area(self):
        di = ConnectedRegionPainter(self.TEST_IMAGE, self.WIDTH, self.HEIGHT)
        di.flood_fill(3, 3, 9)
        expected = [
            [0, 0, 0, 0, 0, 0, 0],
            [0, 1, 2, 1, 3, 1, 0],
            [0, 2, 2, 2, 3, 1, 0],
            [0, 1, 2, 9, 3, 1, 0],
            [0, 3, 3, 3, 3, 1, 0],
            [0, 1, 1, 1, 1, 3, 0],
        ]
        self.assertEqual(di.get_image(), expected)

    def test_flood_fill_connected_region_1(self):
        di = ConnectedRegionPainter(self.TEST_IMAGE, self.WIDTH, self.HEIGHT)
        di.flood_fill(2, 1, 9)
        expected = [
            [0, 0, 0, 0, 0, 0, 0],
            [0, 1, 9, 1, 3, 1, 0],
            [0, 9, 9, 9, 3, 1, 0],
            [0, 1, 9, 1, 3, 1, 0],
            [0, 3, 3, 3, 3, 1, 0],
            [0, 1, 1, 1, 1, 3, 0],
        ]
        self.assertEqual(di.get_image(), expected)

    def test_flood_fill_diagonals_not_connected(self):
        di = ConnectedRegionPainter(self.TEST_IMAGE, self.WIDTH, self.HEIGHT)
        di.flood_fill(1, 4, 8)
        expected = [
            [0, 0, 0, 0, 0, 0, 0],
            [0, 1, 2, 1, 8, 1, 0],
            [0, 2, 2, 2, 8, 1, 0],
            [0, 1, 2, 1, 8, 1, 0],
            [0, 8, 8, 8, 8, 1, 0],
            [0, 1, 1, 1, 1, 3, 0],
        ]
        self.assertEqual(di.get_image(), expected)

    def test_flood_fill_multiple_fills(self):
        di = ConnectedRegionPainter(self.TEST_IMAGE, self.WIDTH, self.HEIGHT)
        di.flood_fill(0, 0, 1)
        di.flood_fill(5, 5, 1)
        di.flood_fill(0, 0, 9)
        expected = [
            [9, 9, 9, 9, 9, 9, 9],
            [9, 9, 2, 9, 3, 9, 9],
            [9, 2, 2, 2, 3, 9, 9],
            [9, 9, 2, 1, 3, 9, 9],
            [9, 3, 3, 3, 3, 9, 9],
            [9, 9, 9, 9, 9, 9, 9],
        ]
        self.assertEqual(di.get_image(), expected)

    def test_flood_fill_5(self):
        di = ConnectedRegionPainter(self.TEST_IMAGE, self.WIDTH, self.HEIGHT)
        # if the coordinates are out of bounds, the method should raise an exception
        with self.assertRaises(ValueError):
            di.flood_fill(7, 7, 9)
        with self.assertRaises(ValueError):
            di.flood_fill(-1, -1, 9)


if __name__ == "__main__":
    unittest.main()
