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
        '''
        Parameters:
            image (list of lists): A 2D array representing the image 
            width (int): The width of the image
            height (int): The height of the image
        Functionality:
            Initializes the ConnectedRegionPainter object, validates that the dimensions of the image match the given width and height
        Returns:
            None
        '''
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
        '''
        Parameters:
            x (int): The x-coordinate
            y (int): The y-coordinate
        Functionality:
            Retrieves the color of the pixel at the specified coordinates
        Returns:
            int: The color value of the pixel
        '''
        if not self.is_valid_pixel(x, y):
            raise ValueError("Out of range")
        return self.image[y][x]

    # method to set the color of a pixel
    def set_pixel(self, x, y, color):
        '''
        Parameters:
            x (int): The x-coordinate
            y (int): The y-coordinate
            color (int): The new color to set for the pixel
        Functionality:
            Updates the color of the pixel at the specified coordinates
        Returns:
            None
        '''
        if not self.is_valid_pixel(x, y):
            raise ValueError("Out of range")
        self.image[y][x] = color

    # method to get the image back
    def get_image(self):
        '''
        Parameters:
            None
        Functionality:
            Retrieves the current state of the image
        Returns:
            list of lists: The 2D array representing the image
        '''
        return self.image

    # method to determine if the pixels are within the image
    def is_valid_pixel(self, x, y):
        '''
        Parameters:
            x (int): The x-coordinate
            y (int): The y-coordinate
        Functionality:
            Checks if the given coordinates are within the bounds of the image
        Returns:
            bool: True if the pixel is valid, False otherwise.
        '''
        return 0 <= x < self.width and 0 <= y < self.height

    # method to flood fill a region with a new color star mode
    def flood_fill(self, x, y, new_color):
        '''
        Parameters:
            x (int): The starting x-coordinate
            y (int): The starting y-coordinate
            new_color (int): The new color to apply to the region
        Functionality:
            Fills the region connected to the starting pixel with the new color
        Returns:
            None
        '''
        if not self.is_valid_pixel(x, y):
            raise ValueError("Out of range")
        target = self.get_pixel(x, y)
        if target == new_color:
            return
        self.star_color(x, y, target, new_color)

    # aux function in case of changing the objective
    def star_color(self, x, y, target, new_color):
        '''
        Parameters:
            x (int): The current x-coordinate
            y (int): The current y-coordinate
            target (int): The original color of the region to fill
            new_color (int): The new color to apply
        Functionality:
            Recursively (this can be change by stack on my 
            research exist many algorithms to do that in 
            case of big images) fills the connected pixels in the region with the new color
        Returns:
            None
        '''
        # ensure that is a valid pixel
        if not self.is_valid_pixel(x, y) or self.get_pixel(x, y) != target:
            return
        # change the actual color of the pixel
        self.set_pixel(x, y, new_color)
        # set directions of the movement of the algorithm
        directions = [(-1, 0), (1, 0), (0, -1), (0, 1)]
        # move on each direction
        for dx, dy in directions:
            # please update the point I forgot it several times debugging
            new_x, new_y = x, y
            # in order to move over the direction create a loop
            while True:
                # make two accumulative values
                new_x += dx
                new_y += dy
                # break in case is outside the border
                if not self.is_valid_pixel(new_x, new_y) or self.get_pixel(new_x, new_y) != target:
                    break
                # recursive call to repeat the same function on the neighborhood
                self.star_color(new_x, new_y, target, new_color)
                self.set_pixel(new_x, new_y, new_color)


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
