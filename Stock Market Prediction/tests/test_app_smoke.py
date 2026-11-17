"""
Smoke Tests for Streamlit Microstructure Terminal App
======================================================
Verifies app initialization, state transitions, radio switching,
and slider interactions using Streamlit's official AppTest harness.
"""

import os
import pytest
from streamlit.testing.v1 import AppTest

APP_PATH = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "app.py")


def test_app_initialization_and_smoke():
    """Verify clean startup without unhandled exceptions."""
    at = AppTest.from_file(APP_PATH)
    at.run(timeout=30)
    assert not at.exception, f"App raised exception on initial startup: {at.exception}"


def test_app_radio_and_slider_interaction():
    """Verify dataset slice toggling and cursor slider navigation."""
    at = AppTest.from_file(APP_PATH)
    at.run(timeout=30)
    assert not at.exception

    # Switch dataset partition to Validation
    if len(at.radio) > 0:
        at.radio[0].set_value("Validation (used for early stopping, optimistic)").run(timeout=30)
        assert not at.exception, f"App raised exception after radio switch: {at.exception}"

    # Move snapshot slider
    if len(at.slider) > 0:
        at.slider[0].set_value(500).run(timeout=30)
        assert not at.exception, f"App raised exception after slider update: {at.exception}"
